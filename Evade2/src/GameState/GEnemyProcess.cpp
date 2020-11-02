//
// Created by Michael Schwartz on 11/2/20.
//

#include "GGame.h"
#include "GEnemyProcess.h"
#include "GEnemyBulletProcess.h"
#include "GCamera.h"
#include "img/enemy_assault_1_img.h"
#include "img/enemy_heavy_bomber_1_img.h"
#include "img/enemy_scout_1_img.h"

#define FIRE_TIME (60 / gGame->mDifficulty + Random(1, 60 / gGame->mDifficulty))

GEnemyProcess::GEnemyProcess() {
  printf("Spawn GEmenyProcess\n");
  mSprite = new GVectorSprite(OTYPE_ENEMY);
  gGameEngine->AddSprite(mSprite);
  respawn();
}

GEnemyProcess::~GEnemyProcess() noexcept {
  mSprite->Remove();
  delete mSprite;
  mSprite = ENull;
}

TBool GEnemyProcess::death() {
  if (mSprite->flags & OFLAG_COLLISION) {
    gGame->mKills++;
    mSprite->flags &= OFLAG_EXPLODE;
    mSprite->mState = 0;
    return ETrue;
  }
  return EFalse;
}

void GEnemyProcess::fire() {
  mSprite->mTimer--;
  if (mSprite->mTimer <= 0) {
    if (GCamera::vx || GCamera::vy) {
      mSprite->mTimer = 1;
      return;
    }
    // FIRE!
//    printf("Fire!!!\n");
    gGameEngine->AddProcess(new GEnemyBulletProcess(mSprite, Random(0, 5) ? EBULLET_BULLET : EBULLET_BOMB));
    mSprite->mTimer = FIRE_TIME;
  } else {
    // TODO: @jay why is this being decremented a second time?
    mSprite->mTimer--;
  }
}

#define DELTA_THETA 8

void GEnemyProcess::bank(TInt16 delta) {
  if (mSprite->flags & BANK_LEFT) {
    mSprite->mTheta -= DELTA_THETA;
    if (mSprite->mTheta < -delta) {
      mSprite->flags &= ~BANK_LEFT;
    }
  } else {
    mSprite->mTheta += DELTA_THETA;
    if (mSprite->mTheta > delta) {
      mSprite->flags |= BANK_LEFT;
    }
  }
}

void GEnemyProcess::respawn() {
  mSprite->mTimer = Random(gGame->mWave > 6 ? 30 : 30, 60) + 30;
//  printf("RESPAWN %d\n", mSprite->mTimer);

  mState = ESTATE_WAITINIT;
}

void GEnemyProcess::init_assault(TBool left) {
  TFloat angle = left ? 0 : (2 * PI);
  mSprite->x      = cos(angle) * 256;
  mSprite->z      = GCamera::z + sin(angle) * 256;
  mSprite->y      = GCamera::y; //  + 64 - random(0, 128);
  mSprite->vx     = mSprite->vy = mSprite->vz = 0;
  mSprite->mState = 0;
  mSprite->mColor = ASSAULT_COLOR;
}

/**
 * Initialize object for scout enemy
 */
void GEnemyProcess::init_scout() {
  mSprite->x      = GCamera::x + Random(-256, 256);
  mSprite->y      = GCamera::y + Random(-256, 256);
  mSprite->z      = GCamera::z + 1024;
  mSprite->vz     = CAMERA_VZ - 12;
  mSprite->vx     = mSprite->vy = 0;
  mSprite->mTheta = Random(-50, 50);
  mSprite->mColor = SCOUT_COLOR;
}

/**
 * Initialize object for bomber enemy
 */
void GEnemyProcess::init_bomber() {
  mSprite->x  = GCamera::x + 128 - Random(0, 127);
  mSprite->y  = GCamera::y + 128 - Random(0, 127);
  mSprite->z  = GCamera::z - 30;
  mSprite->vz = CAMERA_VZ + 1 + gGame->mWave;
  mSprite->vx = mSprite->vy = 0;
  mSprite->mColor = BOMBER_COLOR;
}

void GEnemyProcess::init() {
  mSprite->flags &= ~OFLAG_EXPLODE;
  mSprite->flags &= ~OFLAG_COLLISION;
  mSprite->mTimer = FIRE_TIME;
  mSprite->mTheta = 0;

  // One enemy type enters per wave
  switch (Random(0, (gGame->mWave > 3) ? 3 : gGame->mWave)) {
    case 0:
      mSprite->mLines = (const TInt8 *) &enemy_scout_1_img;
      init_scout();
      mState = ESTATE_SEEK;
      break;
    case 1:
      mSprite->mLines = (const TInt8 *) &enemy_heavy_bomber_1_img;
      init_bomber();
      mState = ESTATE_EVADE;
      break;
    case 2:
      mSprite->mLines = (const TInt8 *) &enemy_assault_1_img;
      init_assault(Random() & 1);
      mState = ESTATE_ORBIT;
      break;
  }
  printf("init\n");
}

TBool GEnemyProcess::StateSeek() {
  GVectorSprite *o = mSprite;
  if (o->BehindCamera()) {
    respawn();
    return ETrue;
  }
  if (death()) {
    mState = ESTATE_EXPLODE;
    return ETrue;
  }
  // bank(o);
  fire();
  o->mTheta += 8;
  if (o->z - GCamera::z < Random(256, 512)) {
    o->mState = -1;
    mState = ESTATE_RUNAWAY;
//    me->sleep(1, run_away);
    return ETrue;
  }

  return ETrue;
}

TBool GEnemyProcess::StateEvade() {
  GVectorSprite *o = mSprite;
  if (o->z - GCamera::z > 512) {
    o->mState = 1;
    mState = ESTATE_RUNAWAY;
//    me->sleep(1, run_away);
    return ETrue;
  }
  if (death()) {
    mState = ESTATE_EXPLODE;
//    me->sleep(1, explode);
    return ETrue;
  }
  bank(15);
  fire();
  return ETrue;
}

TBool GEnemyProcess::StateOrbit() {
  GVectorSprite *o = mSprite;
  if (death()) {
    mState = ESTATE_EXPLODE;
    return ETrue;
  }
  fire();

  if (o->flags & ORBIT_LEFT) {
    o->mState -= gGame->mDifficulty;
    if (o->mState < 0) {
      o->mState = 0;
      o->flags &= ~ORBIT_LEFT;
    }
    else {
      o->mTheta -= 12;
    }
  }
  else {
    o->mState += gGame->mDifficulty;
    if (o->mState > 180) {
      o->mState = 180;
      o->flags |= ORBIT_LEFT;
    }
    else {
      o->mTheta += 12;
    }
  }

  TFloat rad = RADIANS(o->mState);
  o->vy = (GCamera::y > o->y) ? -2 : 2;
  o->y = GCamera::y;
  o->x = cos(rad) * 256;
  if (gGame->GetState() == GAME_STATE_GAME) {
    o->z = GCamera::z + sin(rad) * 256;
  }

//  me->sleep(1);
  return ETrue;
}

TBool GEnemyProcess::StateWaitInit() {
  GVectorSprite *o = mSprite;
  if (o->mTimer <= 0 && gGame->GetState() == GAME_STATE_GAME) {
    init();
    return ETrue;
  }
  else {
    o->mTimer = 1;
  }
  o->mTimer--;
//  me->sleep(1);
  return ETrue;
}

TBool GEnemyProcess::StateRunAway() {
  GVectorSprite *o = mSprite;
  if (gGame->GetState() != GAME_STATE_GAME) {
    o->vz += o->mState * 8;
  }
  else {
    o->vz += o->mState;
  }
  o->vx += o->vx > 0 ? .1 : -.1;
  o->vy += o->vy > 0 ? .1 : -.1;
  if (o->BehindCamera() || (o->z - GCamera::z) > 1024) {
    respawn();
    return ETrue;
  }
  if (death()) {
    mState = ESTATE_EXPLODE;
//    me->sleep(1, explode);
    return ETrue;
  }
  bank();
  fire();
//  me->sleep(1);
  return ETrue;
}

TBool GEnemyProcess::StateExplode() {
  mSprite->flags |= OFLAG_EXPLODE;
  mSprite->mState++;
  if (mSprite->BehindCamera() || mSprite->mState > 50) {
    respawn();
  }
  return ETrue;
}

TBool GEnemyProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyProcess::RunAfter() {
  switch (mState) {
    case ESTATE_SEEK:
      return StateSeek();
    case ESTATE_EVADE:
      return StateEvade();
    case ESTATE_ORBIT:
      return StateOrbit();
    case ESTATE_WAITINIT:
      return StateWaitInit();
    case ESTATE_RUNAWAY:
      return StateRunAway();
    case ESTATE_EXPLODE:
      return StateExplode();
    default:
      Panic("Bad Genemy State %d", mState);
  }
}
