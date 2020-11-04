#ifndef GCAMERA_H
#define GCAMERA_H

#include "Game.h"
#include <common/GVectorSprite.h>

class GCamera {
public:
  static TFloat x,y,z;
  static TFloat vx,vy,vz;

public:
  static void Move() {
    GCamera::x += GCamera::vx;
    GCamera::y += GCamera::vy;
    GCamera::z += GCamera::vz;
  }

  static TBool CollidesWith(GVectorSprite *aVSprite) {
    // If enemy bullet collides with player
    if (abs(aVSprite->z - GCamera::z) < abs(aVSprite->vz) && abs(aVSprite->x - GCamera::x) < 64 && abs(aVSprite->y - GCamera::y) < 64) {
      return ETrue;
    }
    return EFalse;
  }
};



#endif
