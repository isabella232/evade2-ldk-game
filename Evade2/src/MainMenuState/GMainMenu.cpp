//
// Created by Michael Schwartz on 11/4/20.
//

#include "GMainMenu.h"
#include "GStarfield.h"

GMainMenu::GMainMenu() : BGameEngine(gViewPort) {
  mPlayfield = new GStarfield();
}

GMainMenu::~GMainMenu() noexcept {}