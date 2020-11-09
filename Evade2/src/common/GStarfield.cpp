//
// Created by Michael Schwartz on 10/29/20.
//

#include "GStarfield.h"
#include "GCamera.h"

GStarfield::GStarfield() : BPlayfield() {
  for (TInt i = 0; i < NUM_STARS; i++) {
    InitStar(i);
  }
}

GStarfield::~GStarfield() noexcept {

}

void GStarfield::Render() {
  gDisplay.renderBitmap->Clear(0);
  TFloat cz = gCamera->z,
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
    TFloat x      = (sw / 2) - (mStarX[i] - gCamera->x) * ratioX;
    TFloat y      = (sh / 2) - (mStarY[i] - gCamera->y) * ratioY;

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

void GStarfield::InitStar(TInt aIndex) {
  mStarX[aIndex] = TFloat(256) - Random(0, 512) + gCamera->x;
  mStarY[aIndex] = TFloat(256) - Random(0, 512) + gCamera->y;
  mStarZ[aIndex] = gCamera->z + Random(200, 512);
}