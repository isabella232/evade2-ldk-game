//
// Created by Michael Schwartz on 11/4/20.
//

#include "Game.h"
#include "GAttractState.h"
#include "GGame.h"
#include "GEnemyProcess.h"
#include "GAttractProcess.h"
#include "GStarfield.h"
#include "GCamera.h"

GAttractState::GAttractState() : BGameEngine(gViewPort) {
  gGameEngine = this;
//  mPlayfield = new GStarfield();
  AddProcess(new GAttractProcess());
  gCamera->vx = gCamera->vy = 0;
  gCamera->vz = CAMERA_VZ;
}

GAttractState::~GAttractState() noexcept {
}

