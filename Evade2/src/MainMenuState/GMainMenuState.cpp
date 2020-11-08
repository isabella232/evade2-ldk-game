//
// Created by Michael Schwartz on 11/4/20.
//

#include "GMainMenuState.h"
#include "GMainMenuProcess.h"
#include "GStarfield.h"
#include "GCamera.h"

GMainMenuState::GMainMenuState() : BGameEngine(gViewPort) {
//  gGameEngine = this;
  mPlayfield  = new GStarfield();
  gCamera->vx = gCamera->vy = 0;
  gCamera->vz = CAMERA_VZ;
  AddProcess(new GMainMenuProcess());
}

GMainMenuState::~GMainMenuState() noexcept {}