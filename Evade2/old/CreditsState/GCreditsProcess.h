#ifndef GENUS_GCREDITSPROCESS_H
#define GENUS_GCREDITSPROCESS_H

#include "Game.h"

static const TUint TIMEOUT = 2 * FRAMES_PER_SECOND;

class GCreditsProcess : public BProcess {
public:
  GCreditsProcess();
  ~GCreditsProcess() OVERRIDE;

public:
  void ResetTimer();
  TInt CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground);
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

private:
  TInt       mState;
  TInt       mTimer;
  BFont      *mFont16;
  const char *mCredits1[4] = {
    "CRAFTED BY:",
    "MODUS CREATE",
    "DECEMBER 2017",
    0,
  };
  const char *mCredits2[3] = {
    "MUSIC and SFX:",
    "J. GARCIA",
    0
  };
  const char *mCredits3[6] = {
    "ART:",
    "M. TINTIUC",
    "JV DALEN",
    "JD JONES",
    "J. GARCIA",
    0
  };
  const char *mCredits4[5] = {
    "PROGRAMMING:",
    "M. SCHWARTZ",
    "J. GARCIA",
    "M. TINTIUC",
    0
  };
  const char *mCredits5[5] = {
    "PROGRAMMING:",
    "D. BRIGNOLI",
    "S. LEMMONS",
    "A. DENNIS",
    0
  };
  const char *mCredits6[5] = {
    "PROGRAMMING:",
    "V. POPA",
    "L. STILL",
    "G. GRISOGONO",
    0
  };
};

#endif //GENUS_GCREDITSPROCESS_H
