//
// Created by Michael Schwartz on 11/2/20.
//

#ifndef EVADE2_GENEMYPROCESS_H
#define EVADE2_GENEMYPROCESS_H

#include "Game.h"
#include "GVectorSprite.h"

#define ENEMY_ASSAULT 0
#define ENEMY_BOMBER 1
#define ENEMY_SCOUT 2

enum EState {
  ESTATE_SEEK,
  ESTATE_EVADE,
  ESTATE_ORBIT,
  ESTATE_WAITINIT,
  ESTATE_RUNAWAY,
  ESTATE_EXPLODE,
};

class GEnemyProcess : public BProcess {
public:
  GEnemyProcess();
  ~GEnemyProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

private:
  void init_assault(TBool left);
  void init_scout();
  void init_bomber();
  void init();
  void respawn();
  TBool death();
  void fire();
  void bank(TInt16 delta = 45);

protected:
  GVectorSprite *mSprite;

protected:
  TBool StateSeek();
  TBool StateEvade();
  TBool StateOrbit();
  TBool StateWaitInit();
  TBool StateRunAway();
  TBool StateExplode();
protected:
  EState mState;

};


#endif //EVADE2_GENEMYPROCESS_H
