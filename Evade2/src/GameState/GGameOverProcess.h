//
// Created by Michael Schwartz on 11/10/20.
//

#ifndef EVADE2_GGAMEOVERPROCESS_H
#define EVADE2_GGAMEOVERPROCESS_H

#include "Game.h"

class GGameOverProcess : public BProcess {
public:
  GGameOverProcess();
  ~GGameOverProcess();

public:
  TBool RunBefore();
  TBool RunAfter();
protected:
  TFloat mTheta;
  TInt16 mTimer;
};


#endif //EVADE2_GGAMEOVERPROCESS_H
