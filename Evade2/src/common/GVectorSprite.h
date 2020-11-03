#ifndef EVADE2_GVECTORSPRITE_H
#define EVADE2_GVECTORSPRITE_H

#include "BSprite.h"
#include "GResources.h"
#include "Game.h"

struct vec_segment_u8 {
  TInt8 x0;
  TInt8 y0;
  TInt8 x1;
  TInt8 y1;
};

//#define FIRE_TIME (60 / Game::difficulty + random(1, 60 / Game::Game::difficulty))


class GVectorSprite : public BSprite {

public:
  GVectorSprite(TUint64 aType = STYPE_DEFAULT) : BSprite(0 , 0, ENull, aType) {
    mColor = COLOR_WHITE;
    mPad = 0;
    x = y = z = 0;
    vx = vy = vz = 0;
    flags = mState = mTheta = 0;
    mTimer = 0;
    mLines = ENull;
  }

  void StartAnimation() {
    printf("GVectorSprite::StartAnimation()\n");
  }

  TBool DrawVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y, TFloat theta, TFloat scaleFactor, TUint8 color) {
    return ExplodeVectorGraphic(graphic, x, y, theta, scaleFactor, 0, color);
  }

  TBool ExplodeVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                             TFloat theta, TFloat scaleFactor, TInt8 step, TUint8 color);

  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void Move() OVERRIDE {
    x += vx;
    y += vy;
    z += vz;
  }
public:
  void SetLines(const TInt8 *aLines);
  const TInt8 *GetLines() { return mLines; }

protected:
  // if lines is NULL, then the variables in the Object structure can be used
  // for any purpose
  const TInt8 *mLines;

public:

  TFloat z;    // coordinates
  TFloat vz; // velocity in x,y,z
  TInt16 mTimer;
  TInt16 mState; // arbitrary data TInt8 for AI use (can be explosion step, etc.)
  TFloat mTheta; // rotation around Z (in degrees, 0-60)
  TUint8 mColor;
  TUint8 mPad;

//  inline void SetType(TUint8 aType) {
//    mFlags = (mFlags & ~OFLAG_TYPE_MASK) | aType;
//  }

  TBool BehindCamera();
  TFloat DistanceTo(GVectorSprite *aOther);

  TBool do_death() {
//    if (o->flags & OFLAG_COLLISION) {
//      Game::kills++;
//      o->flags &= OFLAG_EXPLODE;
//      o->state = 0;
//      // o->vz = Camera::vz - 3;
//      Sound::play_sound(SFX_ENEMY_EXPLODE);
//      return TRUE;
//    }
    return EFalse;
  }
};


#endif //EVADE2_GVECTORSPRITE_H
