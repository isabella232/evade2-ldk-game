#ifndef GENUS_GMAINMENUPROCESS_H
#define GENUS_GMAINMENUPROCESS_H

#include "Game.h"
#include "GMainMenuContainer.h"

static const TInt TIMEOUT = 5 * FRAMES_PER_SECOND;

class GMainMenuProcess : public BProcess {
public:
  GMainMenuProcess();
  ~GMainMenuProcess() OVERRIDE;

public:
  void ResetTimer();
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

private:
  TBool mShowHighScores;
  TInt32 mTimer;
  GDialogWidget *mContainer;
};

#endif //GENUS_GMAINMENUPROCESS_H
