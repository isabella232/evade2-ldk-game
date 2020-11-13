//
// Created by Michael Schwartz on 11/2/20.
//

#ifndef EVADE2_GPLAYERBULLETPROCESS_H
#define EVADE2_GPLAYERBULLETPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

class GPlayerBulletProcess : public BProcess {
public:
  GPlayerBulletProcess(TFloat deltaX, TFloat deltaY, TBool alt);
  ~GPlayerBulletProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
protected:
  GVectorSprite *mSprite;
  TBool mDying;
};


#endif //EVADE2_GPLAYERBULLETPROCESS_H
