#include "GMainMenuPlayfield.h"



GMainMenuPlayfield::GMainMenuPlayfield(GGameState *aGameState) {
  mGameState = aGameState;
//  mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  gSoundPlayer.PlayMusic(S00_INTRO_XM);
  mTitle = new GVectorTitle(60, 70, "EVADE 2");
}

GMainMenuPlayfield::~GMainMenuPlayfield() {
  // gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT1);
//  delete mFont8;
  delete mFont16;
}
//
//TInt GMainMenuPlayfield::CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
//  TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
//  gDisplay.renderBitmap->DrawString(ENull, s, mFont8, x, aY, aColor, aBackground);
//  return 8;
//}

TInt GMainMenuPlayfield::CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
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


void GMainMenuPlayfield::Animate() {
  mTitle->Move();
}


void GMainMenuPlayfield::Render() {
  mTitle->Render();
  mState++;
}

