#include "GPlayerProcess.h"

#define DEBUGME
//#undef DEBUGME

#include "Game.h"
#include "GCamera.h"
#include "GGameState.h"
#include "GGameOverProcess.h"
#include "GPlayerBulletProcess.h"
#include "img/console_img.h"

enum {
  HUD_SIDE_LEFT,
  HUD_SIDE_RIGHT
};

#define MAX_POWER 100
#define MAX_LIFE 100

#define DEBUGME 1
#undef DEBUGME

GPlayerProcess::GPlayerProcess() {
  color = COLOR_WHITE;
  gCamera->vz = CAMERA_VZ;
  mBoostPower  = MAX_POWER;
  mShield      = MAX_LIFE;
  mNumBullets = 0;
  mAlt        = EFalse;
  mHit        = EFalse;

  //printf("screen %d,%d\n", gDisplay.renderBitmap->Width(), gDisplay.renderBitmap->Height());
}

void GPlayerProcess::DrawPixel(TFloat x, TFloat y) const {
  gDisplay.renderBitmap->WritePixel(x, y, color);
}

void GPlayerProcess::DrawPixel(TFloat x, TFloat y, TUint8 aColor) {
  gDisplay.renderBitmap->WritePixel(x, y, aColor);
}

void GPlayerProcess::DrawLine(TFloat x1, TFloat y1, TFloat x2, TFloat y2) const {
  gDisplay.renderBitmap->DrawLine(gViewPort, x1, y1, x2, y2, color);
}

void GPlayerProcess::DrawBitmap(TInt16 x, TInt16 y, const TUint8 *bitmap, TUint8 w, TUint8 h, TUint8 aColor) const {
  if (x + w < 0 || x > SCREEN_WIDTH - 1 || y + h < 0 || y > SCREEN_HEIGHT - 1) {
    return;
  }
}

void GPlayerProcess::Hit(TInt8 amount) {
  mShield -= (TFloat)amount;
  if (mShield <= 0) {
    gGameEngine->AddProcess(new GGameOverProcess());
  } else {
    mHit = ETrue;
    gSoundPlayer.TriggerSfx(SFX_PLAYER_HIT_WAV, 4);
  }
}

void GPlayerProcess::recharge_shield() {
  if (mShield < MAX_LIFE) {
    mShield += .5;
  }
}

void GPlayerProcess::recharge_power() {
  if (mBoostPower < MAX_POWER) {
    mBoostPower += .5;
  }
}

TBool GPlayerProcess::RunBefore() {
  if (gGameState->mState == STATE_GAME_OVER) {
    return EFalse;
  }
  if (gGame->GetState() != GAME_STATE_GAME) {
    gCamera->vx = gCamera->vy = 0;
    return ETrue;
  }

  if (gGameState->mState == STATE_WARP || gGameState->mState == STATE_NEXT_WAVE) {
    gGame->mStarfield->mBoostSpeed = ETrue;
    gGame->mStarfield->mJSUp = EFalse;
    gGame->mStarfield->mJSDown = EFalse;
    gGame->mStarfield->mJSLeft = EFalse;
    gGame->mStarfield->mJSRight = EFalse;
    return ETrue;
  }
  if (gGameState->mState == STATE_NEXT_WAVE){
    gGame->mStarfield->mJSUp = EFalse;
    gGame->mStarfield->mJSDown = EFalse;
    gGame->mStarfield->mJSLeft = EFalse;
    gGame->mStarfield->mJSRight = EFalse;
    return ETrue;
  }
  else {
    gGame->mStarfield->mBoostSpeed = EFalse;
  }



  TBool jsRight = gControls.IsPressed(JOYRIGHT),
        jsLeft = gControls.IsPressed(JOYLEFT),
        jsUp = gControls.IsPressed(JOYUP),
        jsDown = gControls.IsPressed(JOYDOWN),
        jsRButton = gControls.IsPressed(BUTTONR),
        jsLButton = gControls.IsPressed(BUTTONL);

  // Starfield control
  gGame->mStarfield->mJSUp = jsUp;
  gGame->mStarfield->mJSDown = jsDown;
  gGame->mStarfield->mJSLeft = jsLeft;
  gGame->mStarfield->mJSRight = jsRight;

  if (gControls.WasPressed(CONTROL_FIRE)) {
    if (mNumBullets < MAX_BULLETS) {
      TInt8 deltaX = 0,
            deltaY = 0;

      deltaX = jsRight ? -12 : deltaX;
      deltaX = jsLeft ? 12 : deltaX;

      deltaY = jsUp ? -11 : deltaY;
      deltaY = jsDown ? 13 : deltaY;

      gGameEngine->AddProcess(new GPlayerBulletProcess(deltaX, deltaY, mAlt));

      mAlt = !mAlt;
    }
  }

  if (gControls.WasPressed(CONTROL_BURST)) {
    gSoundPlayer.TriggerSfx(SFX_SPEED_BOOST_WAV, 2);
  }


  if (gControls.IsPressed(CONTROL_BURST)) {
    if (mBoostPower > 0) {
      gGame->mStarfield->mBoostSpeed = ETrue;
      gCamera->vz = CAMERA_WARP_VZ;
      mBoostPower -= .45;
      if (mBoostPower < 0) {
        mBoostPower = 0;
        gSoundPlayer.TriggerSfx(SFX_SPEED_BOOST_END_WAV, 2);
        gGame->mStarfield->mBoostSpeed = EFalse;
        gCamera->vz = CAMERA_VZ;

      }
    } else {
      gCamera->vz = CAMERA_VZ;

    }


  } else {
    if (mBoostPower <= MAX_POWER && gCamera->vz == CAMERA_WARP_VZ) {
      gSoundPlayer.TriggerSfx(SFX_SPEED_BOOST_END_WAV, 2);
    }
    gGame->mStarfield->mBoostSpeed = EFalse;
    gCamera->vz = CAMERA_VZ;
    mBoostPower += .5;
    if (mBoostPower > MAX_POWER) {
      mBoostPower = MAX_POWER;
    }
  }


  //
  if (jsRight) {
    gCamera->vx = -DELTA_CONTROL;
  } else if (jsLeft) {
    gCamera->vx = DELTA_CONTROL;
  } else {
    gCamera->vx = 0;
  }

  if (jsDown) {
    gCamera->vy = DELTA_CONTROL;
  } else if (jsUp) {
    gCamera->vy = -DELTA_CONTROL;
  } else {
    gCamera->vy = 0;
  }
  return ETrue;
}

/************************************************************************/
/** HUD */
/************************************************************************/

void GPlayerProcess::DrawHud(TFloat x, TFloat y) {

  GVectorSprite::DrawVectorGraphic(
    console_img,
    x,
    y,
    0.0,
    .25, // Originally 2.0
    COLOR_WHITE
   );
}


void GPlayerProcess::DrawMeter(TInt8 side, TInt8 value, TInt8 deltaXMeter, TInt8 deltaYMeter) {

  TInt16 y = 160;
  TFloat yStep = 10;

  value /= 10;
  if (side == HUD_SIDE_LEFT) { // LEFT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter, 4 + deltaXMeter + 1,  y + deltaYMeter + 5, COLOR_HUD);
      }
      else {
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter, 4 + deltaXMeter + 4, y + deltaYMeter + 3, COLOR_HUD);
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter + 2, 4 + deltaXMeter + 6, y + deltaYMeter + 3, COLOR_HUD);
      }

      y -= yStep;
    }
  }
  else { // RIGHT (Speed boost)
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        gDisplay.renderBitmap->FillRect(ENull, 313 + deltaXMeter, y + deltaYMeter, 313 + deltaXMeter + 1, y + deltaYMeter + 5, COLOR_HUD);
      }
      else {
        gDisplay.renderBitmap->FillRect(ENull, 310 + deltaXMeter, y + deltaYMeter, 310 + deltaXMeter + 4, y + deltaYMeter + 3, COLOR_HUD);
        gDisplay.renderBitmap->FillRect(ENull, 308 + deltaXMeter, y + deltaYMeter + 2, 308 + deltaXMeter + 6, y + deltaYMeter + 3, COLOR_HUD);
      }

      y -= yStep;
    }
  }
}


TBool GPlayerProcess::RunAfter() {
  if (gGameState->mState == STATE_GAME_OVER) {
    gDisplay.SetColor(0, 0, 0, 0);
    return EFalse;
  }


  if (mHit) {
    gSoundPlayer.TriggerSfx(SFX_PLAYER_HIT_WAV, 4);
    gDisplay.SetColor(COLOR_SPACE, 180, 50, 50);
    gDisplay.renderBitmap->Clear(0);
  } else {
    gDisplay.SetColor(0, 0, 0, 0);
  }
  mHit = EFalse;

  TFloat consoleX         = 0, consoleY = 0, deltaXMeter = 0, deltaYMeter = 0,
         deltaXCrossHairs = 0, deltaYCrossHairs = 0;



  if (gGame->IsGameState() && gGameState->mState != STATE_WARP && gGameState->mState != STATE_NEXT_WAVE) {

    if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
      consoleX         = -4;
      deltaXMeter      = -1;
      deltaXCrossHairs = 4;
    } else if (gControls.IsPressed(CONTROL_JOYLEFT)) {
      consoleX         = 4;
      deltaXMeter      = 1;
      deltaXCrossHairs = -4;
    }

    if (gControls.IsPressed(CONTROL_JOYUP)) {
      consoleY         = -4;
      deltaYMeter      = -1;
      deltaYCrossHairs = 4;
    } else if (gControls.IsPressed(CONTROL_JOYDOWN)) {
      consoleY         = 4;
      deltaYMeter      = 1;
      deltaYCrossHairs = -4;
    }
  }
  else {
    gControls.Reset();
  }


  const TFloat screenMidX = TFloat(SCREEN_WIDTH) / 2, screenMidY = TFloat(SCREEN_HEIGHT) / 2;

  DrawHud(screenMidX + consoleX, (240 + consoleY) - 20);

  /** Reticle **/
  // Top left
  DrawLine(screenMidX + deltaXCrossHairs - 5,
           screenMidY + deltaYCrossHairs,
           screenMidX + deltaXCrossHairs - 12,
           screenMidY + deltaYCrossHairs - 7);

  DrawPixel(screenMidX + deltaXCrossHairs - 5,
            screenMidY + deltaYCrossHairs - 5, 255);

  // Top Right
  DrawLine(screenMidX + deltaXCrossHairs + 5,
           screenMidY + deltaYCrossHairs,
           screenMidX + deltaXCrossHairs + 12,
           screenMidY + deltaYCrossHairs - 7);

  DrawPixel(screenMidX + deltaXCrossHairs + 5,
            screenMidY + deltaYCrossHairs - 5, 255);

  // Bottom Right
  DrawLine(screenMidX + deltaXCrossHairs + 5,
           screenMidY + deltaYCrossHairs,
           screenMidX + deltaXCrossHairs + 12,
           screenMidY + deltaYCrossHairs + 7);

  DrawPixel(screenMidX + deltaXCrossHairs + 5,
            screenMidY + deltaYCrossHairs + 5, 255);

  // Bottom left
  DrawLine(screenMidX + deltaXCrossHairs - 5,
           screenMidY + deltaYCrossHairs,
           screenMidX + deltaXCrossHairs - 12,
           screenMidY + deltaYCrossHairs + 7);

  DrawPixel(screenMidX + deltaXCrossHairs - 5,
            screenMidY + deltaYCrossHairs + 5, 255);

  DrawMeter(0, mShield, deltaXMeter, deltaYMeter);
  DrawMeter(1, mBoostPower, deltaXMeter, deltaYMeter);

#ifdef DEBUGME
  gDisplay.renderBitmap->DrawRect(
    ENull,
    screenMidX - 128,
    screenMidY - 96,
    screenMidX + 128,
    screenMidY + 96,
    COLOR_HUD
  );
#endif

  return ETrue;
}
