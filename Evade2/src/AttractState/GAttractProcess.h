//
// Created by Michael Schwartz on 11/4/20.
//

#ifndef EVADE2_GATTRACTPROCESS_H
#define EVADE2_GATTRACTPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

struct TAttractData {
  TInt8      screen;
  TInt8      x;
  TInt8      y;
  TInt8      offset;
  TInt16     timer;
  TBool      done;
  const char *text;
  TInt8      enemy;
};

class GAttractProcess : public BProcess {
public:
  GAttractProcess();

  ~GAttractProcess();

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  void InitScreen(TInt16 x = 20, TInt16 y = 30) ;
  TBool NextState();
  TBool TypewriterState();

protected:
  TInt mState;
  TAttractData mData;
  GVectorSprite *mSprite;
};


#endif //EVADE2_GATTRACTPROCESS_H
