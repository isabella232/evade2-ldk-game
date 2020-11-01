//
// Created by Michael Schwartz on 10/29/20.
//

#include "GGameState.h"
#include "GGamePlayfield.h"

GGameState::GGameState() : BGameEngine(gViewPort) {
  mPlayfield = new GGamePlayfield();
  // set colors
  gDisplay.SetColor(COLOR_BLACK, 0,0,0);
  for (TInt i=1; i<256; i++) {
    gDisplay.SetColor(i, 255,255,255);
  }
//  gDisplay.SetColor(COLOR_WHITE, 255,255,255);
//  gDisplay.SetColor(COLOR_STAR, 255,255,255);
}

GGameState::~GGameState() {
//  delete mPlayfield;
}


