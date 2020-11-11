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
  gSoundPlayer.PlayMusic(S11_GET_READY_XM);
}

GNextWaveProcess::~GNextWaveProcess() noexcept {
  gGameState->mState = STATE_PLAY;
  gGameState->mKills = 0;
  gGameState->mWave++;
  if (gGameState->mWave % 4 == 0) {
    gGame->mDifficulty++;
  }

  if (gGameState->mWave % 5 == 0) {
    gSoundPlayer.PlayMusic(S09_STAGE_5_XM);
  }
  else if (gGameState->mWave % 4 == 0) {
    gSoundPlayer.PlayMusic(S07_STAGE_4_XM);;
  }
  else if (gGameState->mWave % 3 == 0) {
    gSoundPlayer.PlayMusic(S05_STAGE_3_XM);
  }
  else if (gGameState->mWave % 2 == 0) {
    gSoundPlayer.PlayMusic(S03_STAGE_2_XM);
  }
  else {
    gSoundPlayer.PlayMusic(S01_STAGE_1_XM);
  };


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