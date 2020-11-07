//
// Created by Michael Schwartz on 11/2/20.
//

#include "GGameState.h"
#include "GPlayerProcess.h"
#include "GPlayerBulletProcess.h"
#include "GCamera.h"

#include "img/bullet_img.h"

GPlayerBulletProcess::GPlayerBulletProcess(TFloat deltaX, TFloat deltaY, TBool alt) {
  mSprite = new GVectorSprite(STYPE_PBULLET);
  if (alt) {
    mSprite->x = gCamera->x + 28;
    mSprite->y = gCamera->y - 28;
    mSprite->z = gCamera->z;
    mSprite->mState = 20;
  } else {
    mSprite->x = gCamera->x - 28;
    mSprite->y = gCamera->y - 28;
    mSprite->z = gCamera->z;
    mSprite->mState = -20;
  }
  mSprite->vx = deltaX;
  mSprite->vy = deltaY;
  mSprite->vz = gCamera->vz + BULLET_VZ;
  mSprite->SetLines(bullet_img);
  gGameEngine->AddSprite(mSprite);
}

GPlayerBulletProcess::~GPlayerBulletProcess() noexcept {
  gGameState->mPlayerProcess->mNumBullets--;
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

TBool GPlayerBulletProcess::RunBefore() {
  mSprite->mTheta += mSprite->mState;
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
  if (mSprite->z - gCamera->z > 512) {
    return EFalse;
  }
  BSpriteList        &l = gGameState->mSpriteList;
  for (auto *s = (GVectorSprite *)l.First(); !l.End(s); s = (GVectorSprite *) l.Next(s)) {
    if (s->type & (STYPE_EBULLET | STYPE_ENEMY)) {
      TFloat d = mSprite->DistanceTo(s);
      if (d < COLLISION_RADIUS) {
        s->flags |= OFLAG_COLLISION;
        mSprite->flags |= OFLAG_COLLISION;
      }
    }
  }
  return ETrue;
}