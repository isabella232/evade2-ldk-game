#ifndef EVADE2_GPLAYERPROCESS_H
#define EVADE2_GPLAYERPROCESS_H

// player hit by enemy bullet
#define PLAYER_FLAG_HIT (1 << 0)
// which gun (left/right) to fire from
#define PLAYER_FLAG_ALT (1 << 1)

#include "Game.h"

class GPlayerProcess : public BProcess {
public:
  GPlayerProcess();

public:
  TInt16  power;
  TInt16  shield;
  TInt16  mNumBullets;
//  TUint8 flags;
  TUint8 color;
  TBool  mAlt;
  TBool  mHit;

public:
  void recharge_shield();

  void recharge_power();

  void Hit(TInt8 amount);

  void DrawPixel(TFloat x, TFloat y) const;

  void DrawPixel(TFloat x, TFloat y, TUint8 aColor);

  void DrawLine(TFloat x1, TFloat y1, TFloat x2, TFloat y2) const;

protected:
#ifdef ENABLE_HUD_MOVEMENTS

  void DrawHud(TFloat x, TFloat y);

  void DrawMeter(TInt8 side, TInt8 value, TInt8 deltaXMeter, TInt8 deltaYMeter);

#else
  }
  void DrawMeter(TInt8 side, TInt8 value);
#endif

  void DrawBitmap(TInt16 x, TInt16 y, const TUint8 *bitmap, TUint8 w, TUint8 h, TUint8 aColor = COLOR_WHITE) const;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;
};

#endif
