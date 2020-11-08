#ifndef EVADE2_GVECTORFONT_H
#define EVADE2_GVECTORFONT_H

#include "Game.h"

class GVectorFont{
public:
  GVectorFont();

public:
  TFloat scale;
  TUint8 color; // color to render text in

public:
  // these routine return the width of whatever is printed to the screen
  TInt16 write(TFloat x, TFloat y, char c);
  TInt16 printf(TFloat x, TFloat y, const char *s, ...);
#ifdef ENABLE_ROTATING_TEXT
  TInt16 print_string_rotatedx(TFloat x, TFloat y, TFloat angle, const char *s);
#endif
  TInt16 print_string(TFloat x, TFloat y, char *s);
  TInt16 print_long(TFloat x, TFloat y, TInt64 n, TInt16 base = 10);
//  TInt16 print_float(TInt16 x, TInt16 y, TFloat number, TInt16 digits = 2);
};

extern GVectorFont *gVectorFont;

#endif
