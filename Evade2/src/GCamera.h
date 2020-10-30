#ifndef GCAMERA_H
#define GCAMERA_H

#include "Game.h"
#include <common/GVectorSprite.h>

class GCamera {
public:
  static TFloat mX, mY, mZ;
  static TFloat mVX, mVY, mVZ;

public:
  static void Move() {
    GCamera::mX += GCamera::mVX;
    GCamera::mY += GCamera::mVY;
    GCamera::mZ += GCamera::mVZ;
  }

  static TBool CollidesWith(GVectorSprite *aVSprite) {
    // If enemy bullet collides with player
    if (abs(aVSprite->mZ - GCamera::mZ) < abs(aVSprite->mVZ) && abs(aVSprite->mX - GCamera::mX) < 64 && abs(aVSprite->mY - GCamera::mY) < 64) {
      return ETrue;
    }
    return EFalse;
  }
};



#endif
