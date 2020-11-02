//
// Created by Michael Schwartz on 10/29/20.
//

#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"
#include "GEnemyProcess.h"

GGameState *gGameState;

GGameState::GGameState() : BGameEngine(gViewPort) {
  gGameEngine = this;
  gGameState = this;
  mPlayfield = new GGamePlayfield();
  // set colors
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
  AddProcess(new GEnemyProcess());
  AddProcess(new GEnemyProcess());
  AddProcess(new GEnemyProcess());
  mPlayerProcess = new GPlayerProcess();
  AddProcess(mPlayerProcess);
  gDisplay.SetColor(COLOR_WHITE, 255,255,255);
  gDisplay.SetColor(COLOR_STAR, 255,255,255);
}

GGameState::~GGameState() {
//  delete mPlayfield;
}


