//
// Created by Michael Schwartz on 11/6/20.
//

#ifndef EVADE2_GNEXTWAVEPROCESS_H
#define EVADE2_GNEXTWAVEPROCESS_H

#include "Game.h"

class GNextWaveProcess : public BProcess {
public:
  GNextWaveProcess();

  ~GNextWaveProcess();

public:
  TBool RunBefore();

  TBool RunAfter();
protected:
  TInt mTimer;
};


#endif //EVADE2_GNEXTWAVEPROCESS_H
