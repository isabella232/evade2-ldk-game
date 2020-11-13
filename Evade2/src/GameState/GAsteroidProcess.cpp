#include "GAsteroidProcess.h"
#include "GGameState.h"
#include "GCamera.h"
#include "GPlayerProcess.h"

#include "img/environment_asteroid_img.h"

enum {
  STATE_DELAY,
  STATE_LOOP,
};

GAsteroidProcess::GAsteroidProcess() : BProcess() {
  mSprite = new GVectorSprite();
  gGameState->AddSprite(mSprite);
  mState = STATE_DELAY;
  mAlt   = EFalse;
}

GAsteroidProcess::~GAsteroidProcess() {

}

void GAsteroidProcess::InitRock() {
  if (mAlt) {
    // off right side
    mSprite->x      = gCamera->x + (512. - Random(0, 512));
    mSprite->mState = -4;
  } else {
    mSprite->x      = gCamera->x - (512. - Random(0, 512));
    mSprite->mState = 4;
  }
  mAlt = !mAlt;
  mSprite->y  = gCamera->y + Random(-100, 100);
  mSprite->z  = gCamera->z + 768;
  mSprite->vz = CAMERA_VZ - 4;
  mSprite->SetLines(environment_asteroid_img_large);
}

void GAsteroidProcess::Respawn() {
  mSprite->SetLines(ENull);
  mSprite->mTimer = Random(0, 120) + 60;
  mState = STATE_DELAY;
}

TBool GAsteroidProcess::Clipped() {
  if (mSprite->mState > 0) {
    if (mSprite->x > gCamera->x + 1024) {
      return ETrue;
    }
  } else {
    if (mSprite->x < gCamera->x - 1024) {}
    return ETrue;
  }
  if (mSprite->z < gCamera->z || mSprite->z > gCamera->z + 768) {
    return ETrue;
  }
  return EFalse;
}

TBool GAsteroidProcess::RunBefore() {
  return ETrue;
}

TBool GAsteroidProcess::DelayState() {
  if (--mSprite->mTimer < 0) {
    InitRock();
    mState = STATE_LOOP;
  }
  return ETrue;
}

TBool GAsteroidProcess::LoopState() {
  if (mSprite->flags * OFLAG_COLLISION) {
    // absorb player bullets
    mSprite->flags &= ~OFLAG_COLLISION;
  }

  if (gCamera->CollidesWith(mSprite)) {
    if (gGameState->mState == STATE_PLAY) {
      gGameState->mPlayerProcess->Hit(30);
    }
    // bounce
    mSprite->vz = gCamera->vz + 2;
    mSprite->z  = gCamera->z + 10;
    mSprite->vx = 8 - Random(0, 16);
  } else if (Clipped()) {
    Respawn();
    return ETrue;
  }
  mSprite->mTheta += 8;
  mSprite->vy = gCamera->vy / 2;

  return ETrue;
}

TBool GAsteroidProcess::RunAfter() {
  switch (mState) {
    case STATE_DELAY:
      return DelayState();
    case STATE_LOOP:
      return LoopState();
  }
  return ETrue;
}
