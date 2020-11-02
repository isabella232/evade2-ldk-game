//
// Created by Michael Schwartz on 11/2/20.
//

#ifndef EVADE2_GENEMYBULLETPROCESS_H
#define EVADE2_GENEMYBULLETPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

#define EBULLET_BULLET 1
#define EBULLET_BOMB 2

class GEnemyBulletProcess : public BProcess {
public:
  GEnemyBulletProcess(GVectorSprite *enemy, TInt8 type);

  ~GEnemyBulletProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  GVectorSprite *mSprite;
};


#endif //EVADE2_GENEMYBULLETPROCESS_H
