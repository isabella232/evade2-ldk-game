#include "GPlayerProcess.h"

#define DEBUGME
//#undef DEBUGME

#include "Game.h"
#include "GCamera.h"
#include "GGameState.h"
#include "GGameOverProcess.h"
#include "GPlayerBulletProcess.h"
#include "img/hud_console_img.h"

//TODO: Put in own files
const TUint8 crosshair_left_4x8[] = {
  // width, height4, 8,
  0x81, 0x42, 0x24, 0x99
};

// crosshair_right.png
// 4x8
const TUint8 crosshair_right_4x8[] = {
  // width, height 4, 8,
  0x99, 0x24, 0x42, 0x81
};

#define MAX_POWER 100
#define MAX_LIFE 100

GPlayerProcess::GPlayerProcess() {
  color = COLOR_WHITE;
  gCamera->vz = CAMERA_VZ;
  power       = MAX_POWER;
  shield      = MAX_LIFE;
  mNumBullets = 0;
  mAlt        = EFalse;
  mHit        = EFalse;
  printf("screen %d,%d\n", gDisplay.renderBitmap->Width(), gDisplay.renderBitmap->Height());
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
#if 0
  // no need to draw at all if we're offscreen
  if (x + w < 0 || x > WIDTH - 1 || y + h < 0 || y > HEIGHT - 1)
    return;

  return;
  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  int rows = h / 8;
  if (h % 8 != 0)
    rows++;
  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT / 8) - 1)
      break;
    if (bRow > -2) {
      for (int iCol = 0; iCol < w; iCol++) {
        if (iCol + x > (WIDTH - 1))
          break;
        if (iCol + x >= 0) {
          const TUint8 bmp_bits = pgm_read_byte(bitmap + (a * w) + iCol);
          if (bRow >= 0) {
            if (color == WHITE)
              sBuffer[(bRow * WIDTH) + x + iCol] |= bmp_bits << yOffset;
            else if (color == BLACK)
              sBuffer[(bRow * WIDTH) + x + iCol] &= ~(bmp_bits << yOffset);
            else
              sBuffer[(bRow * WIDTH) + x + iCol] ^= bmp_bits << yOffset;
          }
          if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2) {
            if (color == WHITE)
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] |= bmp_bits >> (8 - yOffset);
            else if (color == BLACK)
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] &= ~(bmp_bits >> (8 - yOffset));
            else
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] ^= bmp_bits >> (8 - yOffset);
          }
        }
      }
    }
  }
#endif
}

void GPlayerProcess::Hit(TInt8 amount) {
  shield -= amount;
  if (shield <= 0) {
    gGameEngine->AddProcess(new GGameOverProcess());
    // ProcessManager::birth(GameOver::entry);
  } else {
    mHit = ETrue;
    // Sound::play_sound(SFX_PLAYER_HIT_BY_ENEMY);
  }
}

void GPlayerProcess::recharge_shield() {
  if (shield < MAX_LIFE) {
    shield++;
  }
}

void GPlayerProcess::recharge_power() {
  if (power < MAX_POWER) {
    power++;
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

  if (gControls.WasPressed(CONTROL_FIRE)) {
    if (mNumBullets < MAX_BULLETS) {
      TInt8 deltaX = 0,
            deltaY = 0;

      deltaX = gControls.IsPressed(CONTROL_JOYRIGHT) ? -12 : deltaX;
      deltaX = gControls.IsPressed(CONTROL_JOYLEFT) ? 12 : deltaX;

      deltaY = gControls.IsPressed(CONTROL_JOYUP) ? -11 : deltaY;
      deltaY = gControls.IsPressed(CONTROL_JOYDOWN) ? 13 : deltaY;

      gGameEngine->AddProcess(new GPlayerBulletProcess(deltaX, deltaY, mAlt));

      mAlt = !mAlt;
    }
  }

  if (gControls.IsPressed(CONTROL_BURST)) {
    if (power > 0) {
      gCamera->vz = CAMERA_WARP_VZ;
      power--;
      if (power < 0) {
        power = 0;
      }
    } else {
      gCamera->vz = CAMERA_VZ;
    }
  } else {
    gCamera->vz = CAMERA_VZ;
    power++;
    if (power > MAX_POWER) {
      power = MAX_POWER;
    }
  }

  if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
    gCamera->vx = -DELTACONTROL;
  } else if (gControls.IsPressed(CONTROL_JOYLEFT)) {
    gCamera->vx = DELTACONTROL;
  } else {
    gCamera->vx = 0;
  }

  if (gControls.IsPressed(CONTROL_JOYDOWN)) {
    gCamera->vy = DELTACONTROL;
  } else if (gControls.IsPressed(CONTROL_JOYUP)) {
    gCamera->vy = -DELTACONTROL;
  } else {
    gCamera->vy = 0;
  }
  return ETrue;
}

/************************************************************************/
/** HUD */
/************************************************************************/

void GPlayerProcess::DrawHud(TFloat x, TFloat y) {

  const TUint8 color   = COLOR_WHITE;
  const TFloat width   = 0x30, height = 0x08;
  const TUint8 *bitmap = hud_console_img;

  for (TInt xx = 0, xxx = 0; xx < width; xx++, xxx += 2) {
    for (TInt yy = 0, yyy = 0; yy < height; yy++, yyy += 2) {
      if (y + yy > SCREEN_HEIGHT - 1) {
        continue;
      }
      TInt8 byte = bitmap[xx + (yy / 8)];
      TInt8 bit  = (yy % 8);
      if (byte & (1 << bit)) {
        //        Graphics::drawPixel(x + xx, y + yy, color);
        DrawPixel(x + xxx, y + yyy, COLOR_HUD);
        DrawPixel(x + xxx + 1, y + yyy, COLOR_HUD);
        DrawPixel(x + xxx, y + yyy + 1, COLOR_HUD);
        DrawPixel(x + xxx + 1, y + yyy + 1, COLOR_HUD);
        //        Graphics::drawPixel(x + xx * 2 + 1, y + yy * 2 + 1, color);
      }
    }
  }
}

#ifdef ENABLE_HUD_MOVEMENTS

// 13 == full. Anything less, and we draw "less meter"
void GPlayerProcess::DrawMeter(TInt8 side, TInt8 value, TInt8 deltaXMeter, TInt8 deltaYMeter) {

  //start at X:14
  // Draw 2 lines, skip one line, iterate 13 total times
  // if left, X:0, else X:128
  // Y Step is 3

  // TODO: Tighten up!
  TInt8 y = 45;
  value /= 10;
  if (side == 0) { // LEFT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(1 + deltaXMeter, y + deltaYMeter);
        DrawPixel(1 + deltaXMeter, y + 1 + deltaYMeter);
      } else {
        DrawLine(1 + deltaXMeter, y + deltaYMeter, 3 + deltaXMeter, y + deltaYMeter);
        DrawLine(1 + deltaXMeter, y + 1 + deltaYMeter, 4 + deltaXMeter, y + 1 + deltaYMeter);
      }
      y -= 3;
    }
  } else { // RIGHT
    const TInt RIGHT_SIDE = SCREEN_WIDTH - 2;
    for (TInt8 i          = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(RIGHT_SIDE + deltaXMeter, y + deltaYMeter);
        DrawPixel(RIGHT_SIDE + deltaXMeter, y + 1 + deltaYMeter);
      } else {
        DrawLine(RIGHT_SIDE - 2 + deltaXMeter, y + deltaYMeter, RIGHT_SIDE + deltaXMeter, y + deltaYMeter);
        DrawLine(RIGHT_SIDE - 3 + deltaXMeter, y + 1 + deltaYMeter, RIGHT_SIDE + deltaXMeter, y + 1 + deltaYMeter);
      }
      y -= 3;
    }

  }
}

#else
// TODO: @Jay - we should probably remove this code/condition.  This else code is not touched/ported to CE

// 13 == full. Anything less, and we draw "less meter"
void GPlayerProcess::DrawMeter(TInt8 side, TInt8 value) {

  //start at X:14
  // Draw 2 lines, skip one line, iterate 13 total times
  // if left, X:0, else X:128
  // Y Step is 3

  // TODO: Tighten up!
  TInt8 y = 45;
  value /= 10;
  if (side == 0) { // LEFT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(0, y);
        DrawPixel(0, y + 1);
      } else {
        DrawLine(0, y, 2, y);
        DrawLine(0, y + 1, 3, y + 1);
      }
      y -= 3;
    }
  } else { // RIGHT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(SCREEN_WIDTH-1, y);
        DrawPixel(SCREEN_WIDTH-1, y + 1);
      } else {
        DrawLine(SCREEN_WIDTH-2, y, SCREEN_WIDTH, y);
        DrawLine(SCREEN_WIDTH-3, y + 1, SCREEN_WIDTH, y + 1);
      }
      y -= 3;
    }
  }
}

#endif // #if ENABLE_HUD_MOVEMENTS

TBool GPlayerProcess::RunAfter() {
  if (gGameState->mState == STATE_GAME_OVER) {
    return EFalse;
  }
  if (mHit) {
    gDisplay.SetColor(0, 255, 255, 255);
  } else {
    gDisplay.SetColor(0, 0, 0, 0);
  }
  mHit = EFalse;

#ifdef ENABLE_HUD_MOVEMENTS
  TFloat consoleX         = 0, consoleY = 0, deltaXMeter = 0, deltaYMeter = 0,
         deltaXCrossHairs = 0, deltaYCrossHairs = 0;


  if (gGame->IsGameState()) {

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

  const TFloat screenMidX = TFloat(SCREEN_WIDTH) / 2, screenMidY = TFloat(SCREEN_HEIGHT) / 2;

  DrawHud(screenMidX - (0x30) + consoleX, (240 + consoleY) - 12);

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

  DrawMeter(0, shield, deltaXMeter, deltaYMeter);
  DrawMeter(1, power, deltaXMeter, deltaYMeter);

#else
  DrawBitmap(40, 58, hud_console_img, 0x30, 0x08);

  DrawMeter(0, shield);
  DrawMeter(1, power);
#endif

  return ETrue;
}
