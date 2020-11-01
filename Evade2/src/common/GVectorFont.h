#ifndef EVADE2_GVECTORFONT_H
#define EVADE2_GVECTORFONT_H

#include "Game.h"

class GVectorFont{
public:
  GVectorFont();

public:
  TUint16 scale; // 8.8 fixed point
  TUint8 color; // color to render text in

public:
  // these routine return the width of whatever is printed to the screen
  TInt8 write(TInt8 x, TInt8 y, char c);
  TInt8 printf(TInt8 x, TInt8 y, const char *s, ...);
#ifdef ENABLE_ROTATING_TEXT
  TInt8 print_string_rotatedx(TInt8 x, TInt8 y, TFloat angle, const char *s);
#endif
  TInt8 print_string(TInt8 x, TInt8 y, char *s);
  TInt8 print_long(TInt8 x, TInt8 y, TInt64 n, TInt8 base = 10);
  TInt8 print_float(TInt8 x, TInt8 y, TFloat number, TInt8 digits = 2);
};

#endif
