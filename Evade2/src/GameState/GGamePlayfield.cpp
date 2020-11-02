//
// Created by Michael Schwartz on 10/29/20.
//

#include "GGamePlayfield.h"
#include "GCamera.h"

GGamePlayfield::GGamePlayfield() : BPlayfield() {
  for (TInt i = 0; i < NUM_STARS; i++) {
    InitStar(i);
  }
}

GGamePlayfield::~GGamePlayfield() noexcept {

}

void GGamePlayfield::Render() {
  gDisplay.renderBitmap->Clear(0);
  TFloat cz = GCamera::z,
         sw = TFloat(SCREEN_WIDTH),
         sh = TFloat(SCREEN_HEIGHT);

  for (int i = 0; i < NUM_STARS; i++) {
    TFloat zz = (mStarZ[i] - cz) * 2;
    if (zz < 0) {
      InitStar(i);
      zz = (mStarZ[i] - cz) * 2;
    }
    TFloat ratioX = sw / (zz + sw);
    TFloat ratioY = sh / (zz + sh);
    TFloat x      = (sw / 2) - (mStarX[i] - GCamera::x) * ratioX;
    TFloat y      = (sh / 2) - (mStarY[i] - GCamera::y) * ratioY;

    if (x < 0) {
//      printf("InitStar x %f < 0\n", x);
      InitStar(i);
    } else if (x > sw) {
//      printf("InitStar x %f > %f\n", x, sw);
      InitStar(i);
    } else if (y < 0) {
//      printf("InitStar y %f < 0\n", y);
      InitStar(i);
    } else if (y > sh) {
//      printf("InitStar y %f > %f\n", y, sh);
      InitStar(i);
    }
//    printf("Plot %f,%f\n", x, y);
    gDisplay.renderBitmap->SafeWritePixel(x, y, COLOR_STAR);
  }
}

void GGamePlayfield::InitStar(TInt aIndex) {
  mStarX[aIndex] = TFloat(256) - Random(0, 512) + GCamera::x;
  mStarY[aIndex] = TFloat(256) - Random(0, 512) + GCamera::y;
  mStarZ[aIndex] = GCamera::z + Random(200, 512);
}