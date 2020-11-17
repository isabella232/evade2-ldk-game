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
  mSprite = new GVectorSprite(STYPE_ENEMY);
  gGameState->AddSprite(mSprite);
  mSprite->flags &= ~OFLAG_COLLISION;

  mSprite->SetLines(ENull);
  mState = STATE_DELAY;
  mSprite->mTimer = Random(0, 120) + 60;
  mAlt   = EFalse;
  mRotationSpeed =  Random(-5, 5);
}

GAsteroidProcess::~GAsteroidProcess() {

}

void GAsteroidProcess::InitRock() {
  mAlt = !mAlt;

  if (mAlt) {
    // off right side
    mSprite->x      = gCamera->x + (512. - Random(0, 128));
    mSprite->mState = -4;
  } else {
    mSprite->x      = gCamera->x - (512. - Random(0, 128));
    mSprite->mState = 4;
  }
  mSprite->y  = gCamera->y + Random(-50, 50);
  mSprite->z  = gCamera->z + 768;
  mSprite->vz = CAMERA_VZ - 2;
  mSprite->vx = 0;
  mSprite->vy = 0;
  mSprite->SetLines(environment_asteroid_img_large);
}

void GAsteroidProcess::Respawn() {
  mSprite->SetLines(ENull);
  mSprite->mTimer = 1;// Random(0, 120) + 60;
  mState = STATE_DELAY;
}

TBool GAsteroidProcess::Clipped() {
  if (mSprite->mState > 0) {
    if (mSprite->x > gCamera->x + 1024) {
      return ETrue;
    }
  } else {
    if (mSprite->x < gCamera->x - 1024) {
      return ETrue;
    }
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
//  if (mSprite->flags * OFLAG_COLLISION) {
//    // absorb player bullets
//    mSprite->flags &= ~OFLAG_COLLISION;
//  }

//  printf("GAsteroidProcess->mSprite->vz %2f\n", mSprite->vz);

  if (gCamera->CollidesWith(mSprite)) {
    if (gGameState->mState == STATE_PLAY) {
      gGameState->mPlayerProcess->Hit(30);
    }
    // bounce
    mSprite->vz = gCamera->vz + 2;
    mSprite->z  = gCamera->z + 10;
    mSprite->vx = Random(-8, 8);
    mSprite->vy = Random(-8, 8);
    mRotationSpeed *= -1;
  } else if (Clipped()) {
    Respawn();
    return ETrue;
  }
  mSprite->mTheta += mRotationSpeed;
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
