#ifndef GENUS_GCREDITSPLAYFIELD_H
#define GENUS_GCREDITSPLAYFIELD_H

#include "Game.h"
#include "GPlayerSprite.h"
#include "GVectorTitle.h"

class GCreditsPlayfield : public BPlayfield {
public:
  GCreditsPlayfield(GGameState *aGameState);
  ~GCreditsPlayfield() OVERRIDE;

  void Animate() OVERRIDE;
  void Render() OVERRIDE;

protected:
  GGameState *mGameState;
  GVectorTitle *mTitle;
};

#endif //GENUS_GCREDITSPLAYFIELD_H
