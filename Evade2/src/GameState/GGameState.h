//
// Created by Michael Schwartz on 10/29/20.
//

#ifndef EVADE2_GGAMESTATE_H
#define EVADE2_GGAMESTATE_H

#include <BGameEngine.h>

enum {
  STATE_PLAY,
  STATE_BOSS,
  STATE_NEXT_WAVE,
  STATE_GAME_OVER
};

class GPlayerProcess;

class GGameState : public BGameEngine {
public:
  GGameState();

  ~GGameState() OVERRIDE;

public:
  void PostRender() OVERRIDE;

public:
  GPlayerProcess *mPlayerProcess;
  TInt32         mState;
  TInt16         mKills;
  TInt16         mWave;
};

extern GGameState *gGameState;

#endif //EVADE2_GGAMESTATE_H
