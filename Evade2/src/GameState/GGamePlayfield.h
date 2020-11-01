//
// Created by Michael Schwartz on 10/29/20.
//

#ifndef EVADE2_GGAMEPLAYFIELD_H
#define EVADE2_GGAMEPLAYFIELD_H

#include <BPlayfield.h>

const int NUM_STARS = 128;

class GGamePlayfield : public BPlayfield {
public:
  GGamePlayfield();

  ~GGamePlayfield();

public:
  void Render() OVERRIDE;

protected:
  void InitStar(TInt aIndex);

protected:
  TFloat mStarX[NUM_STARS], mStarY[NUM_STARS], mStarZ[NUM_STARS];
};


#endif //EVADE2_GGAMEPLAYFIELD_H
