//
// Created by Michael Schwartz on 11/6/20.
//

#include "GNextWaveProcess.h"
#include "GPlayerProcess.h"
#include "GGameState.h"
#include "GCamera.h"

GNextWaveProcess::GNextWaveProcess() : BProcess() {
  mTimer      = 240;
  gCamera->vz = CAMERA_VZ;
  gGameState->mState = STATE_NEXT_WAVE;
}

GNextWaveProcess::~GNextWaveProcess() noexcept {
  gGameState->mState = STATE_PLAY;
  gGameState->mKills = 0;
  gGameState->mWave++;
  if (gGameState->mWave % 4 == 0) {
    gGame->mDifficulty++;
  }
  gCamera->vz = CAMERA_VZ;
}

TBool GNextWaveProcess::RunBefore() {
  return ETrue;
}

TBool GNextWaveProcess::RunAfter() {
  gVectorFont->scale = 1.5;
  gVectorFont->printf(85, ALERT_TOP, "START WAVE %d", gGameState->mWave + 1);
  gGameState->mPlayerProcess->recharge_shield();
  gGameState->mPlayerProcess->recharge_power();
  return --mTimer > 0;
}