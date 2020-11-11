//
// Created by Michael Schwartz on 10/29/20.
//

#include "GGameState.h"
#include "GStarfield.h"
#include "GPlayerProcess.h"
#include "GEnemyProcess.h"
#include "GNextWaveProcess.h"
#include "GBossProcess.h"

GGameState *gGameState;

GGameState::GGameState() : BGameEngine(gViewPort) {
  gGameEngine = this;
  gGameState  = this;
  mState      = STATE_PLAY;
  mWave       = 1;
  mKills      = 0;
//  mPlayfield  = new GStarfield();
  // set colors
  AddProcess(new GEnemyProcess());
  AddProcess(new GEnemyProcess());
  AddProcess(new GEnemyProcess());
  mPlayerProcess = new GPlayerProcess();
  AddProcess(mPlayerProcess);
}

GGameState::~GGameState() {
//  delete mPlayfield;
}

void GGameState::PostRender() {
  if (mState != STATE_PLAY) {
    return;
  }
  if (mKills > (10 + mWave) * gGame->mDifficulty) {
    AddProcess(new GBossProcess());
  }
}

