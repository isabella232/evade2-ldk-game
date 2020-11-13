//
// Created by Michael Schwartz on 11/6/20.
//

#ifndef EVADE2_GBOSSPROCESS_H
#define EVADE2_GBOSSPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

class GBossProcess : public BProcess {
public:
  GBossProcess();

  ~GBossProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  void InitOrbit();

  TBool Hit();

  void EngagePlayerRandomXY();

  void EngagePlayerOrbit();

  void RandomizeFlee();

  void EngagePlayerFlee();

protected:
  TBool WarpState();

  TBool ExplodeState();

//  TBool StartActionState();
  TBool ActionState();

protected:
  GVectorSprite *mSprite;
  const TInt8   *mLines;
  TInt          mType;
  TInt         mBlink;
  TInt          mTimer;
  TInt          mColor, mDeltaColor;

protected:
  void SetState(TInt aNewState);

  TInt mState;
protected:
  TInt mHitPoints;
};

#endif //EVADE2_GBOSSPROCESS_H
