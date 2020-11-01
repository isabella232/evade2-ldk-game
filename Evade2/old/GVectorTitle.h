#ifndef EVADE2_GVECTORTITLE_H
#define EVADE2_GVECTORTITLE_H


#include "Game.h"
#include "GVectorSprite.h"
#include "Font.h"
#include "string.h"

class GVectorTitle : public GVectorSprite{

public:
  GVectorTitle(TInt16 aX, TInt16 aY, const char *aTitle = "WTF")
  : GVectorSprite(){
    SetTitle(aTitle);
    mX = aX;
    mY = aY;
    mColorRed = 150;
    mColorRedStep = 3;
  }

  void Move() OVERRIDE {

    mTheta += 1;
//    if (mTheta > 90 + 360 * 2) {
//      mTheta = 90 + 360 * 2;
//    }

    mColorRed += mColorRedStep;
    if (mColorRed > 255) {
      mColorRedStep *= -1;
    }
    if (mColorRed < 150) {
      mColorRedStep *= -1;
    }
  }

  void Render() {
    Font::scale = 2 * 0x200;
    gDisplay.renderBitmap->SetColor(SHMOO_COLOR, (TInt8)mColorRed, 50, 50);

    Font::print_string_rotatedx(SHMOO_COLOR, mX, mY, mTheta, mTitle);
    Font::scale = 0x100;
  }

  void SetTitle(const char* aTitle) {
    strcpy(mTitle, aTitle);
  }

  char mTitle[64];
  TFloat mColorRed;
  TInt8 mColorRedStep;
  TInt16 mX;
  TInt16 mY;
};


#endif //EVADE2_GVECTORTITLE_H
