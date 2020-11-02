//
// Created by Michael Schwartz on 11/2/20.
//

#include "GGameState.h"
#include "GPlayerProcess.h"
#include "GPlayerBulletProcess.h"
#include "GCamera.h"

#include "img/bullet_img.h"

const TFloat BULLET_VZ = 15;

GPlayerBulletProcess::GPlayerBulletProcess(TFloat deltaX, TFloat deltaY, TBool alt) {
  mSprite = new GVectorSprite(OTYPE_PLAYER_BULLET);
  if (alt) {
    mSprite->x = GCamera::x + 28;
    mSprite->y = GCamera::y - 28;
    mSprite->z = GCamera::z;
    mSprite->mState = 20;
  } else {
    mSprite->x = GCamera::x - 28;
    mSprite->y = GCamera::y - 28;
    mSprite->z = GCamera::z;
    mSprite->mState = -20;
  }
  mSprite->vx = deltaX;
  mSprite->vy = deltaY;
  mSprite->vz = GCamera::vz + BULLET_VZ;
  mSprite->mLines = bullet_img;
  gGameEngine->AddSprite(mSprite);
}

GPlayerBulletProcess::~GPlayerBulletProcess() noexcept {
  printf("BUllet death\n");
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
  if (mSprite->z - GCamera::z > 512) {
    return EFalse;
  }
  return ETrue;
}