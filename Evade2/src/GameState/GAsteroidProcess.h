#ifndef EVADE2_GASTEROIDPROCESS_H
#define EVADE2_GASTEROIDPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

class GAsteroidProcess : public BProcess {
public:
  GAsteroidProcess();
  ~GAsteroidProcess();

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
protected:
  TBool DelayState();
  TBool LoopState();

  void Respawn();
  TBool Clipped();
  void InitRock();

protected:
  GVectorSprite *mSprite;
  TInt mState;
  TBool mAlt;
};


#endif //EVADE2_GASTEROIDPROCESS_H
