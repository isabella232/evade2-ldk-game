//
// Created by Michael Schwartz on 10/29/20.
//

#ifndef EVADE2_GGAMESTATE_H
#define EVADE2_GGAMESTATE_H

#include <BGameEngine.h>

class GPlayerProcess;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState();

public:
//  void GameLoop() OVERRIDE;
  GPlayerProcess *mPlayerProcess;
};

extern GGameState *gGameState;

#endif //EVADE2_GGAMESTATE_H
