#ifndef EVADE2_GATTRACTMODESTATE_H
#define EVADE2_GATTRACTMODESTATE_H

#include "Game.h"
#include "GEnemySprite.h"
enum {
  ATTRACT_STATE_SCOUT,
  ATTRACT_STATE_BOMBER,
  ATTRACT_STATE_ASSAULT,
};

class GAttractModeState : public BGameEngine {

public:
  EXPLICIT GAttractModeState();
  virtual ~GAttractModeState();
  void PreRender();
  void PostRender();

private:
  TInt16 mState;
  GEnemySprite *mEnemySprite;
};



#endif //EVADE2_GATTRACTMODESTATE_H
