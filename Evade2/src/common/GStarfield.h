//
// Created by Michael Schwartz on 10/29/20.
//

#ifndef EVADE2_GSTARFIELD_H
#define EVADE2_GSTARFIELD_H

#include <BPlayfield.h>

const int NUM_STARS = 128;

class GStarfield : public BPlayfield {
public:
  GStarfield();

  ~GStarfield();

public:
  void Render() OVERRIDE;

protected:
  void InitStar(TInt aIndex);

protected:
  TFloat mStarX[NUM_STARS], mStarY[NUM_STARS], mStarZ[NUM_STARS];
};


#endif //EVADE2_GSTARFIELD_H
