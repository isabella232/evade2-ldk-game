//
// Created by Michael Schwartz on 11/2/20.
//

#include "GGameState.h"
#include "GEnemyBulletProcess.h"
#include "GPlayerProcess.h"
#include "GCamera.h"
#include "img/ebomb_img.h"
#include "img/ebullet_img.h"

GEnemyBulletProcess::GEnemyBulletProcess(GVectorSprite *enemy, TInt8 type) {
  const TFloat FRAMES = 90 / gGame->mDifficulty;
  mSprite = new GVectorSprite(STYPE_EBULLET);
  mSprite->SetLines( (type == EBULLET_BOMB) ? ebomb_img : ebullet_img);
  mSprite->mColor = (type == EBULLET_BOMB) ? BOMB_COLOR : EBULLET_COLOR;
  mSprite->mTimer = 256; // timeout
  mSprite->x      = enemy->x - 8;
  mSprite->y      = enemy->y - 8;
  mSprite->z      = enemy->z;
  mSprite->vx = (GCamera::x - mSprite->x) / FRAMES;
  mSprite->vy = (GCamera::y - mSprite->y) / FRAMES;
  mSprite->vz = GCamera::vz - (mSprite->z - GCamera::z) / FRAMES;
//  printf("Bullet vx,vy,vz = %f,%f,%f mState(%d)\n", mSprite->vx, mSprite->vy, mSprite->vz, mSprite->mTimer);
  gGameEngine->AddSprite(mSprite);
}

GEnemyBulletProcess::~GEnemyBulletProcess() noexcept {
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

TBool GEnemyBulletProcess::RunBefore() {
  mSprite->mTheta += (mSprite->GetLines() == ebomb_img) ? mSprite->x : 40;
  return ETrue;
}

TBool GEnemyBulletProcess::RunAfter() {
  if (GCamera::CollidesWith(mSprite)) {
    if (gGame->IsGameState()) {
      gGameState->mPlayerProcess->Hit(10);
    }
  }

  if (mSprite->BehindCamera()) {
    return EFalse;
  }
  if (--mSprite->mTimer <= 0) {
    return EFalse;
  }
  return ETrue;
}