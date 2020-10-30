#include "Game.h"
#include "GSplashPlayfield.h"
#include "GSplashProcess.h"

GSplashState::GSplashState() : BGameEngine(gViewPort) {
  mPlayfield = new GSplashPlayfield();
  AddProcess(new GSplashProcess());
}

GSplashState::~GSplashState() = default;

