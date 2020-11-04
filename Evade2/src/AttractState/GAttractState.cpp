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
  mPlayfield = new GStarfield();
  AddProcess(new GAttractProcess());
  GCamera::vx = GCamera::vy = 0;
  GCamera::vz = 2;

  gDisplay.SetColor(COLOR_BLACK, 0,0,0);
  for (TInt i=1; i<256; i++) {
    gDisplay.SetColor(i, 255,255,255);
  }
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);
  gDisplay.SetColor(ASSAULT_COLOR, 255, 50, 50);
  gDisplay.SetColor(BOMBER_COLOR, 50, 255, 50);
  gDisplay.SetColor(SCOUT_COLOR, 255, 50, 255);
  gDisplay.SetColor(EBULLET_COLOR, 50, 50, 255);
  gDisplay.SetColor(BOMB_COLOR, 255, 255, 50);
}

GAttractState::~GAttractState() noexcept {
}

