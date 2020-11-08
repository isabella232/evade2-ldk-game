//
// Created by Michael Schwartz on 11/4/20.
//

#include "GMainMenuProcess.h"
#include "GCamera.h"

GMainMenuProcess::GMainMenuProcess() : BProcess() {
//  gCamera->vx = 0;
//  gCamera->vy = 0;
//  gCamera->vz = CAMERA_VZ;
  mTimer = 280 * 2;
  mTheta = 90;
}

GMainMenuProcess::~GMainMenuProcess() noexcept {}

TBool GMainMenuProcess::RunAfter() {
  gVectorFont->scale = 4;
  gVectorFont->color = COLOR_SHMOO;
  gVectorFont->print_string_rotatedx(70, 90, mTheta, "EVADE2");
  gVectorFont->color = COLOR_WHITE;

  mTimer--;
  gVectorFont->scale = 1;
  if (mTimer & 32) {
    gVectorFont->scale = 2;
    gVectorFont->printf(130, 155, "START");
  }

 mTheta += 5;
  if (mTheta > 90 + 360 * 2) {
    mTheta = 90 + 360 * 2;
  }

  return ETrue;
}

TBool GMainMenuProcess::RunBefore() {
  if (gControls.WasPressed(BUTTON_START)) {
    gGame->SetState(GAME_STATE_GAME);
    return EFalse;
  }
  if (mTimer < 0 || gControls.WasPressed(BUTTONA|BUTTONB)) {
    gGame->SetState(GAME_STATE_ATTRACT_MODE);
    return EFalse;
  }
  return ETrue;
}
