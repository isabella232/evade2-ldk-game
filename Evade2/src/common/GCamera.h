#ifndef GCAMERA_H
#define GCAMERA_H

#include "Game.h"
#include <common/GVectorSprite.h>

class GCamera {
public:
  GCamera();

public:
  TFloat x, y, z;
  TFloat vx, vy, vz;

public:
  void Move() {
    x += vx;
    y += vy;
    z += vz;
  }

  TBool CollidesWith(GVectorSprite *aVSprite) const {
    // If enemy bullet collides with player
    if (abs(aVSprite->z - z) <= abs(aVSprite->vz) && abs(aVSprite->x - x) <= 100 && abs(aVSprite->y - y) <= 96) {
      return ETrue;
    }
    return EFalse;
  }
};

extern GCamera *gCamera;

#endif
