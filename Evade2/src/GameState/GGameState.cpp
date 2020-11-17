//
// Created by Michael Schwartz on 10/29/20.
//

#include "GGameState.h"
#include "GStarfield.h"
#include "GPlayerProcess.h"
#include "GEnemyProcess.h"
#include "GNextWaveProcess.h"
#include "GBossProcess.h"
#include "GAsteroidProcess.h"

GGameState *gGameState;

GGameState::GGameState() : BGameEngine(gViewPort) {
  gGameEngine = this;
  gGameState  = this;
  mState      = STATE_PLAY;
  mWave       = 1;
  mKills      = 0;
//  mPlayfield  = new GStarfield();

//  AddProcess(new GEnemyProcess());
  AddProcess(new GEnemyProcess());
//  AddProcess(new GAsteroidProcess());
  AddProcess(new GEnemyProcess());
  mPlayerProcess = new GPlayerProcess();
  AddProcess(mPlayerProcess);
  gSoundPlayer.PlayMusic(S01_STAGE_1_XM);
}

GGameState::~GGameState() {
//  delete mPlayfield;
}

void GGameState::PostRender() {
  gGame->mStarfield->mWarp = (mState == STATE_WARP);

  if (mState != STATE_PLAY) {
    return;
  }
//  if (mKills > (10 + mWave) * gGame->mDifficulty) {

  if (mKills > 0) {
    AddProcess(new GBossProcess());
    gSoundPlayer.TriggerSfx(SFX_ALERT_WAV, 2);
  }
}

