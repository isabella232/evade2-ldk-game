#ifndef MODITE_GSTARFIELDPROCESS_H
#define MODITE_GSTARFIELDPROCESS_H

#include "BProcess.h"
#include "Game.h"
static const TInt8 NUM_STARS = 90;

//
//bool faster;
//bool slower;

static TUint32 actualTime = 0;
static TInt32 speedMills;

static const TInt16 STAR_SPEED_MIN = 5; // Minimum movement in pixels per update. (value is inclusive)
static const TInt16 STAR_SPEED_MAX = 20; // Maximum movement in pixels per update. (value is inclusive)

static const TUint16 RANDOM_Z_MIN = 10;
static const TUint16 RANDOM_Z_MAX = 600;


class GStar {
public:
  GStar() {
    mX = 0;
    mY = 0;
    mZ = 0;
    mSpeed = 0;
    mScreenX = 0;
    mScreenY = 0;
    mOldScreenX = 0;
    mOldScreenY = 0;
  }

  void Randomize(TInt16 xMin, TInt16 xMax, TInt16 yMin, TInt16 yMax, TInt16 zMin, TInt16 zMax, TInt16 speedMin, TInt16 speedMax)  {
    mX = Random(xMin, xMax);
    mY = Random(yMin, yMax);
    mZ = Random(zMin, zMax);
    mSpeed = Random(speedMin, speedMax + 1);
  }
  TFloat mX, mY, mZ, mSpeed;
  TInt16 mScreenX, mScreenY, mOldScreenX, mOldScreenY;
};



static GStar stars[NUM_STARS];  // An array of star instances.
static TUint8 colors[NUM_STARS];

class GStarFieldProcess {
public:
  explicit GStarFieldProcess() {
    mCurrSpeed = 10;
    mMinSpeed = 1;
    mBoostSpeed = false;
    mWarp = false;

    for (TInt i = 0; i < NUM_STARS; i++) {
      InitStar(i, 0);
    }
  }

  ~GStarFieldProcess() = default;

  void Render() {


    float travelX = 0,
        travelY = 0,
        travelZ = 0;

    // Todo: this needs to be in Player.cpp or GGame.cpp
    TBool jsRight = gControls.IsPressed(JOYRIGHT),
        jsLeft  = gControls.IsPressed(JOYLEFT),
        jsUp    = gControls.IsPressed(JOYUP),
        jsDown  = gControls.IsPressed(JOYDOWN),
        jsRButton = gControls.IsPressed(BUTTONR),
        jsLButton = gControls.IsPressed(BUTTONL);


    //Todo: ROTATE via L or R Buttons in Z dimension
//    if (gGame.GetState == GAME_STATE_GAME) {
//      // rotate
      if (jsUp) {
        travelY = .02;
      }

      if (jsDown) {
        travelY = -.02;
      }

      if (jsLeft) {
        travelZ = -.02;
      }

      if (jsRight) {
        travelZ = .02;
      }

//    }


    if (mWarp) {
      travelX = .03;
    }

    // Loop through each star.
    for (int i = 0; i < NUM_STARS; i++) {
      stars[i].mZ -= mCurrSpeed;

      if (travelY != 0) {
        float temp_y = stars[i].mY;
        float temp_z = stars[i].mZ;
        stars[i].mY = temp_y * cos(travelY) - temp_z * sin(travelY);
        stars[i].mZ = temp_z * cos(travelY) + temp_y * sin(travelY);
      }

      if (travelX != 0) {
        float temp_x = stars[i].mX;
        float temp_y = stars[i].mY;
        stars[i].mX = temp_x * cos(travelX) - temp_y * sin(travelX);
        stars[i].mY = temp_y * cos(travelX) + temp_x * sin(travelX);
      }

      if (travelZ != 0) {
        float temp_x = stars[i].mX;
        float temp_z = stars[i].mZ;
        stars[i].mX = temp_x * cos(travelZ) - temp_z * sin(travelZ);
        stars[i].mZ = temp_z * cos(travelZ) + temp_x * sin(travelZ);
      }

      stars[i].mScreenX = stars[i].mX / stars[i].mZ * 100 + SCREEN_WIDTH / 2;
      stars[i].mScreenY = stars[i].mY / stars[i].mZ * 100 + SCREEN_HEIGHT / 2;

      actualTime = Milliseconds();
      if (mBoostSpeed && mCurrSpeed <= STAR_SPEED_MAX && actualTime - speedMills >= 25) {
        mCurrSpeed = mCurrSpeed + 5;
        speedMills = actualTime;
      }

      if (!mBoostSpeed && mCurrSpeed > STAR_SPEED_MIN && actualTime - speedMills >= 10) {
        mCurrSpeed -= 5;
        speedMills = actualTime;
      }
      if (mCurrSpeed < STAR_SPEED_MIN) {
        mCurrSpeed = STAR_SPEED_MIN;
      }


      //If the stars go off the screen remove them and re-draw. If the stars hang out in the center remove them also
      if (stars[i].mScreenX > SCREEN_WIDTH || stars[i].mScreenX < 0 || stars[i].mScreenY > SCREEN_HEIGHT || stars[i].mScreenY < 0 || (stars[i].mScreenX == SCREEN_WIDTH >> 1 && stars[i].mScreenY == SCREEN_HEIGHT >> 1) ) {
        int xMin = -1000,
            xMax = 1000,
            yMin = -500,
            yMax = 500;

//        if ((gGame.GetState == GAME_STATE_GAME) && (jsLeft || jsRight)) {
//          xMin = -1000;
//          xMax = 1000;
//        }

        stars[i].Randomize(
            xMin,
            xMax,
            yMin,
            yMax,
            RANDOM_Z_MIN,
            RANDOM_Z_MAX,
            STAR_SPEED_MIN,
            STAR_SPEED_MAX
        );

        stars[i].mScreenX = stars[i].mX / stars[i].mZ * 100 + SCREEN_WIDTH / 2;
        stars[i].mScreenY = stars[i].mY / stars[i].mZ * 100 + SCREEN_HEIGHT / 2;
        stars[i].mOldScreenX = stars[i].mScreenX;
        stars[i].mOldScreenY = stars[i].mScreenY;
      }


      // Draw the star at its new coordinate.
      gDisplay.renderBitmap->DrawLine(
        ENull,
        stars[i].mScreenX,
        stars[i].mScreenY,
        stars[i].mOldScreenX,
        stars[i].mOldScreenY,
        STAR_COLOR
      );
//      Gfx::DrawLine(
//          stars[i].mScreenX,
//          stars[i].mScreenY,
//          stars[i].mOldScreenX,
//          stars[i].mOldScreenY,
//          colors[i]
//      );

      //keep track of the old spot
      stars[i].mOldScreenX = stars[i].mScreenX;
      stars[i].mOldScreenY = stars[i].mScreenY;
    }

  };

  void Animate() {
//    for (TInt i = 0; i < NUM_STARS; i++) {
//      if (mStarX[i] < -10) {
//        InitStar(i);
//      }
//      else {
//        mStarX[i] -= mStarSpeed[i];
//      }
//    }
  }


protected:

  void InitStar(TInt aIndex, TInt32 aXStartIndex = 319) {
    stars[aIndex].Randomize(
      -500,
      1000,
      -500,
      1000,
      0,
      1,
      STAR_SPEED_MIN,
      STAR_SPEED_MAX
    );
//    colors[aIndex] = 255 - Random(0,128);

  }

//  TFloat mStarX[NUM_STARS]{};
//  TFloat mStarY[NUM_STARS]{};
//  TFloat mStarZ[NUM_STARS]{};
//  TFloat mStarWidth[NUM_STARS]{};
//  TFloat mStarSpeed[NUM_STARS]{};
  TInt mCurrSpeed;
  TInt mMinSpeed;
  TBool mWarp;
  TBool mBoostSpeed;
};


#endif //MODITE_GSTARFIELDPROCESS_H
