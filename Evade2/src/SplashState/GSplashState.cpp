#include "Game.h"
#include "GSplashPlayfield.h"
#include "GSplashProcess.h"
#include "GSplashState.h"

GSplashState::GSplashState() : BGameEngine(gViewPort) {
  gGameEngine = this;
  mPlayfield = new GSplashPlayfield();
  AddProcess(new GSplashProcess());
}

GSplashState::~GSplashState()  {
//  delete mPlayfield;
//  mPlayfield = ENull;
}

