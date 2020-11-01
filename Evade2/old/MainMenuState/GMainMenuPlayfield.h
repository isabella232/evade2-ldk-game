#ifndef GENUS_GMAINMENUPLAYFIELD_H
#define GENUS_GMAINMENUPLAYFIELD_H

#include "Game.h"
#include "GPlayerSprite.h"
#include "GVectorTitle.h"

class GMainMenuPlayfield : public BPlayfield {
public:
  GMainMenuPlayfield(GGameState *aGameState);
  ~GMainMenuPlayfield() OVERRIDE;

//  TInt CenterText8(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);
  TInt CenterText16(const char *s, TInt aY, TInt aColor = COLOR_TEXT, TInt aBackground = -1);

  void Animate() OVERRIDE;
  void Render() OVERRIDE;
public:

//  BFont *mFont8;
  BFont *mFont16;
  TInt  mState;

protected:

  GGameState *mGameState;
  GVectorTitle *mTitle;
};

#endif //GENUS_GMAINMENUPLAYFIELD_H
