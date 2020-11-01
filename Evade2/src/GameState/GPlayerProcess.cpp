#include "GPlayerProcess.h"

#define DEBUGME
//#undef DEBUGME

#include "Game.h"
#include "GCamera.h"

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
  GCamera::mVZ = CAMERA_VZ;
  power = MAX_POWER;
  shield = MAX_LIFE;
  num_bullets = 0;
  flags = 0;
}

void GPlayerProcess::DrawPixel(TFloat x, TFloat y) {
  gDisplay.renderBitmap->WritePixel(x, y, color);
}

void GPlayerProcess::DrawLine(TFloat x1, TFloat y1, TFloat x2, TFloat y2) {
  gDisplay.renderBitmap->DrawLine(gViewPort, x1, y1, x2, y2, color);
}

void GPlayerProcess::DrawBitmap(TInt16 x, TInt16 y, const TUint8 *bitmap, TUint8 w, TUint8 h, TUint8 color) {
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

void GPlayerProcess::hit(TInt8 amount) {
  shield -= amount;
  if (shield <= 0) {
    // ProcessManager::birth(GameOver::entry);
  }
  else {
    flags |= PLAYER_FLAG_HIT;
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
  if (gGame->GetState() == GAME_STATE_GAME) {
    // if (game_mode != MODE_GAME) {
    GCamera::mVX = GCamera::mVY = 0;
    return ETrue;
  }

  if (gControls.WasPressed(CONTROL_FIRE)) {
    TInt8 deltaX = 0,
          deltaY = 0;

    deltaX = gControls.IsPressed(CONTROL_JOYRIGHT) ? -12 : deltaX;
    deltaX = gControls.IsPressed(CONTROL_JOYLEFT) ? 12 : deltaX;

    deltaY = gControls.IsPressed(CONTROL_JOYUP) ? -11 : deltaY;
    deltaY = gControls.IsPressed(CONTROL_JOYDOWN) ? 13 : deltaY;

    // Bullet::fire(deltaX, deltaY, Player::flags & PLAYER_FLAG_ALT);
    flags ^= PLAYER_FLAG_ALT;
  }

  if (gControls.IsPressed(CONTROL_BURST)) {
    if (power > 0) {
      GCamera::mVZ = CAMERA_VZ * 2;
      power--;
      if (power < 0) {
        power = 0;
      }
    }
    else {
      GCamera::mVZ = CAMERA_VZ;
    }
  }
  else {
    GCamera::mVZ = CAMERA_VZ;
    power++;
    if (power > MAX_POWER) {
      power = MAX_POWER;
    }
  }

  if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
    GCamera::mVX = -DELTACONTROL;
  }
  else if (gControls.IsPressed(CONTROL_JOYLEFT)) {
    GCamera::mVX = DELTACONTROL;
  }
  else {
    GCamera::mVX = 0;
  }

  if (gControls.IsPressed(CONTROL_JOYDOWN)) {
    GCamera::mVY = DELTACONTROL;
  }
  else if (gControls.IsPressed(CONTROL_JOYUP)) {
    GCamera::mVY = -DELTACONTROL;
  }
  else {
    GCamera::mVY = 0;
  }
  return ETrue;
}

/************************************************************************/
/** HUD */
/************************************************************************/

#ifdef ENABLE_HUD_MOVEMENTS

// 13 == full. Anything less, and we draw "less meter"
static void drawMeter(TInt8 side, TInt8 value, TInt8 deltaXMeter, TInt8 deltaYMeter) {

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
      }
      else {
        DrawLine(1 + deltaXMeter, y + deltaYMeter, 3 + deltaXMeter, y + deltaYMeter);
        DrawLine(1 + deltaXMeter, y + 1 + deltaYMeter, 4 + deltaXMeter, y + 1 + deltaYMeter);
      }
      y -= 3;
    }
  }
  else { // RIGHT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(126 + deltaXMeter, y + deltaYMeter);
        DrawPixel(126 + deltaXMeter, y + 1 + deltaYMeter);
      }
      else {
        DrawLine(124 + deltaXMeter, y + deltaYMeter, 126 + deltaXMeter, y + deltaYMeter);
        DrawLine(123 + deltaXMeter, y + 1 + deltaYMeter, 126 + deltaXMeter, y + 1 + deltaYMeter);
      }
      y -= 3;
    }
  }
}

#else

// 13 == full. Anything less, and we draw "less meter"
void GPlayerProcess::drawMeter(TInt8 side, TInt8 value) {

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
      }
      else {
        DrawLine(0, y, 2, y);
        DrawLine(0, y + 1, 3, y + 1);
      }
      y -= 3;
    }
  }
  else { // RIGHT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        DrawPixel(127, y);
        DrawPixel(127, y + 1);
      }
      else {
        DrawLine(126, y, 128, y);
        DrawLine(125, y + 1, 128, y + 1);
      }
      y -= 3;
    }
  }
}

#endif // #if ENABLE_HUD_MOVEMENTS

TBool GPlayerProcess::RunAfter() {
  // TODO: swap background black -> white -> black
  // arduboy.invert(flags & PLAYER_FLAG_HIT);
  flags &= ~PLAYER_FLAG_HIT;

#ifdef ENABLE_HUD_MOVEMENTS
  TInt8 consoleX = 40,
        consoleY = 58,
        deltaXMeter = 0,
        deltaYMeter = 0,
        deltaXCrossHairs = 0,
        deltaYCrossHairs = 0;

  // Font::scale = .6 * 256;
  // Font::printf(5,5,"D %d", Game::difficulty);
  // Font::printf(5,12,"W %d", Game::wave);
  // Font::scale = 256;
  if (game_mode == MODE_GAME) {

    if (gControls.IsPressed(JOYSTICK_RIGHT)) {
      consoleX = 38;
      deltaXMeter = -1;
      deltaXCrossHairs = 4;
    }
    else if (gControls.IsPressed(JOYSTICK_LEFT)) {
      consoleX = 42;
      deltaXMeter = 1;
      deltaXCrossHairs = -4;
    }

    if (gControls.IsPressed(JOYSTICK_UP)) {
      consoleY = 56;
      deltaYMeter = -1;
      deltaYCrossHairs = 4;
    }
    else if (gControls.IsPressed(JOYSTICK_DOWN)) {
      consoleY = 60;
      deltaYMeter = 1;
      deltaYCrossHairs = -4;
    }
  }

  DrawBitmap(consoleX, consoleY, hud_console_img, 0x30, 0x08);
  // // DrawLine(64, 0, 64, 64); // used to measure the center of the screen.

  DrawBitmap(53 + deltaXCrossHairs, 30 + deltaYCrossHairs, crosshair_left_4x8, 4, 8);
  DrawBitmap(72 + deltaXCrossHairs, 30 + deltaYCrossHairs, crosshair_right_4x8, 4, 8);

  drawMeter(0, shield, deltaXMeter, deltaYMeter);
  drawMeter(1, power, deltaXMeter, deltaYMeter);

#else
  DrawBitmap(40, 58, hud_console_img, 0x30, 0x08);

  drawMeter(0, shield);
  drawMeter(1, power);
#endif
  return ETrue;
}
