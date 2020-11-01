#ifndef EVADE2_GPLAYERPROCESS_H
#define EVADE2_GPLAYERPROCESS_H

// player hit by enemy bullet
#define PLAYER_FLAG_HIT (1 << 0)
// which gun (left/right) to fire from
#define PLAYER_FLAG_ALT (1 << 1)

#include "Game.h"
#include <BProcess.h>

class GPlayerProcess : public BProcess {
public:
  GPlayerProcess();

public:
  TInt8 power;
  TInt8 shield;
  TInt8 num_bullets;
  TUint8 flags;
  TUint8 color;

public:
  void recharge_shield();
  void recharge_power();

  void hit(TInt8 amount);
  void DrawPixel(TFloat x, TFloat y);
  void DrawLine(TFloat x1, TFloat y1, TFloat x2, TFloat y2);

protected:
  void drawMeter(TInt8 side, TInt8 value);
  void DrawBitmap(TInt16 x, TInt16 y, const TUint8 *bitmap, TUint8 w, TUint8 h, TUint8 color = COLOR_WHITE);

public:
  TBool RunBefore();
  TBool RunAfter();
};
#endif
