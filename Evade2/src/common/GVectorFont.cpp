#include "GVectorFont.h"

#define DEBUGME

/**
 * Some of this code lifted from Arduino serial Print class and modified.
 * See: https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.cpp
 */

#include "Game.h"
#include "charset.h"

GVectorFont *gVectorFont;

// constructor
GVectorFont::GVectorFont() {
  scale = 1.0;
  color = COLOR_TEXT;
  gVectorFont = this;
}

#ifdef ENABLE_ROTATING_TEXT
TInt16 GVectorFont::print_string_rotatedx(TFloat x, TFloat y, TFloat theta, const char *s) {
  theta = TFloat(theta) * 3.1415926 / 180;
  TFloat cost = cos(theta),
         sint = sin(theta);
  const char *p = s;

  const TInt16 size = 9;

  TInt16 xo = x;
  while (char c = *p++) {
    auto *glyph = (const TInt8 *)charset[toupper(c) - 32];
    if (glyph) {
      TInt8 lines = *glyph++;

      for (TInt16 i = 0; i < lines; i++) {
        TFloat x0 = *glyph++ * scale + x,
               y0 = *glyph++ * scale + y,
               x1 = *glyph++ * scale + x,
               y1 = *glyph++ * scale + y;

        gDisplay.renderBitmap->DrawLine(
          gViewPort,
          x0,
          ((y0 - y) * sint + cost + y),
          x1,
          ((y1 - y) * sint + cost + y),
          color);
      }
      x += size * scale;
    }
    else {
      x += 6 * scale;
    }
  }
  return x - xo;
}
#endif

TInt16 GVectorFont::write(TFloat x, TFloat y, char c) {
  const TInt8 *glyph;
  const TInt16 width = 9;

  glyph = (TInt8 *)charset[toupper(c) - 32];
  if (glyph) {
    TInt8 lines = *glyph++;

    for (TInt16 i = 0; i < lines; i++) {
      TInt8 x0 = *glyph++,
            y0 = *glyph++,
            x1 = *glyph++,
            y1 = *glyph++;

      gDisplay.renderBitmap->DrawLine(
        gViewPort,
        x + x0 * scale, y + y0 * scale,
        x + x1 * scale, y + y1 * scale,
        color);
    }
  }
  return width * scale;
}

TInt16 GVectorFont::print_string(TFloat x, TFloat y, char *s) {
  TInt16 xx = x;
  while (char c = *s++) {
    TInt16 width = write(x, y, c);
    x += width;
  }
  return x - xx; // width of string printed
}

TInt16 GVectorFont::print_long(TFloat x, TFloat y, TInt64 n, TInt16 base) {
  char buf[8 * sizeof(TInt64) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2)
    base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return print_string(x, y, str);
}

#ifdef PRINTF_TFloat
TInt16 GVectorFont::print_float(TInt16 x, TInt16 y, double number, TInt16 digits) {
  TInt16 xx = x;
  if (isnan(number)) {
    x += write(x, y, 'n');
    x += write(x, y, 'a');
    x += write(x, y, 'n');
    return x;
  }
  if (isinf(number)) {
    x += write(x, y, 'i');
    x += write(x, y, 'n');
    x += write(x, y, 'f');
    return x;
  }
  if (number > 4294967040.0 || number < -4294967040.0) {
    x += write(x, y, 'o');
    x += write(x, y, 'v');
    x += write(x, y, 'f');
    return x;
  }

  // Handle negative numbers
  if (number < 0.0) {
    x += write(x, y, '-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  x += print_long(x, y, int_part, 10);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    x += write(x, y, '.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    x += print_long(x, y, toPrint, 10);
    remainder -= toPrint;
  }

  return x - xx;
}
#endif

TInt16 GVectorFont::printf(TFloat x, TFloat y, const char *s, ...) {
  va_list ap;
  TInt16 xx = x;
  char c;
  const char *p = s;
  va_start(ap, s);

  while ((c = *p++)) {
    if (c == '%') {
      c = *p++;
      switch (c) {
        case '\0':
          va_end(ap);
          return x - xx;
#ifdef PRINTF_TFloat
        case '%':
          x += write(x, y, '%');
          break;
        case 'f':
          x += print_float(x, y, va_arg(ap, double));
          break;
#endif
        case 'd':
          x += print_long(x, y, (unsigned long)va_arg(ap, int));
          break;
#ifdef PRINTF_TFloat
        case 'x':
          x += print_long(x, y, (unsigned long)va_arg(ap, int) & 0xffff, 16);
          break;
        case 'l':
          x += print_long(x, y, va_arg(ap, long));
          break;
#endif
        default:
          x += write(x, y, c);
          break;
      }
    }
    else {
      x += write(x, y, c);
    }
  }
  va_end(ap);
  return xx - x;
}
