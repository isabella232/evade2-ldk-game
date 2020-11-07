//
// Created by Michael Schwartz on 11/4/20.
//

#ifndef EVADE2_GMAINMENUPROCESS_H
#define EVADE2_GMAINMENUPROCESS_H

#include "Game.h"

class GMainMenuProcess : public BProcess {
public:
  GMainMenuProcess();

  ~GMainMenuProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  TFloat mTheta;
  TInt32 mTimer;
};


#endif //EVADE2_GMAINMENUPROCESS_H
