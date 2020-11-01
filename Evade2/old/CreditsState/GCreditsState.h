#ifndef GENUS_GCREDITSSTATE_H
#define GENUS_GCREDITSSTATE_H

#include "Game.h"

class GCreditsState : public BGameEngine {
public:
  GCreditsState(GGameState *aGameState);
  virtual ~GCreditsState();

private:
  BFont *mFont16;
};

#endif //GENUS_GCREDITSSTATE_H
