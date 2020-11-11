//
// Created by Michael Schwartz on 11/10/20.
//

#include "GGameOverProcess.h"
#include "GGameState.h"
#include "GMainMenuState.h"
#include "GCamera.h"

GGameOverProcess::GGameOverProcess() : BProcess() {
  gGameState->mState = STATE_GAME_OVER;
  mTheta = 0;
  mTimer = 200;
  gCamera->vx = gCamera->vy = 0;
  gCamera->vz = CAMERA_VZ;
  gControls.Reset();
  gSoundPlayer.PlayMusic(S10_GAME_OVER_XM);
}

GGameOverProcess::~GGameOverProcess() noexcept = default;

TBool GGameOverProcess::RunBefore() {
  if (mTimer-- < 0) {
    gGame->SetState(GAME_STATE_MAIN_MENU);
    return EFalse;
  }
  return ETrue;
}

TBool GGameOverProcess::RunAfter() {
  mTheta += 6;
  gVectorFont->scale = 3;
  gVectorFont->color = COLOR_SHMOO;
  gVectorFont->print_string_rotatedx(53, 80, mTheta, "GAME OVER");
  gVectorFont->scale = 1.5;
  gVectorFont->color = COLOR_WHITE;
  gVectorFont->printf(gGameState->mWave < 9 ? 50 : 45, 160, "WAVES SURVIVED: %d", gGameState->mWave - 1);
  return ETrue;
}
