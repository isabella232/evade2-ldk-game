#include "GCreditsProcess.h"

GCreditsProcess::GCreditsProcess() : BProcess() {
  mState = 0;
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  ResetTimer();
}

GCreditsProcess::~GCreditsProcess() {
  delete mFont16;
}

void GCreditsProcess::ResetTimer() {
  mTimer = TIMEOUT;
}

TInt GCreditsProcess::CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
  TInt width = aBackground == -1 ? 12 : 16;
  TInt x     = TInt((SCREEN_WIDTH - (strlen(s) * width)) / 2);
  if (aBackground != -1) {
    gDisplay.renderBitmap->DrawString(ENull, s, mFont16, x, aY, aColor, TInt16(aBackground));
  }
  else {
    gDisplay.renderBitmap->DrawString(ENull, s, mFont16, x, aY, aColor, TInt16(aBackground), -4);
  }
  return 16;
}

TBool GCreditsProcess::RunBefore() {
  return ETrue;
}

TBool GCreditsProcess::RunAfter() {
  if (gControls.WasPressed(BUTTON_START)) {
    gGame->SetState(GAME_STATE_MAIN_OPTIONS);
    return EFalse;
  }

  if (mTimer-- == 0 || gControls.WasPressed(BUTTON_ANY | JOYRIGHT)) {
    ResetTimer();
    if (++mState > 5) {
      mState = 0;
    }
  } else if (gControls.WasPressed(JOYLEFT)) {
    ResetTimer();
    if (--mState < 0) {
      mState = 5;
    }
  }

  const char **ptr;
  switch (mState) {
    case 0:
      ptr = mCredits1;
      break;
    case 1:
      ptr = mCredits2;
      break;
    case 2:
      ptr = mCredits3;
      break;
    case 3:
      ptr = mCredits4;
      break;
    case 4:
      ptr = mCredits5;
      break;
    case 5:
      ptr = mCredits6;
      break;
  }

  TInt y = 120;
  while (*ptr != 0) {
    y += CenterText16(*ptr, y, COLOR_WHITE, -1);
    ++ptr;
  }

  return ETrue;
}
