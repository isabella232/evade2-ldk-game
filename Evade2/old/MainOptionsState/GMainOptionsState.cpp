#include "Game.h"
#include "GMusicWidget.h"
#include "GSfxWidget.h"
// #include "GResetWidget.h"
#include "GExitWidget.h"
#include "GVectorTitle.h"
#include "common/GButtonWidget.h"

const TUint8 WIDGET_CONTAINER_Y = 60;

class OptionsContainer : public GDialogWidget {
public:
    OptionsContainer(TInt aX, TInt aY) : GDialogWidget("Options", aX, aY) {
      AddWidget((BWidget &) *new GMusicWidget());
      AddWidget((BWidget &) *new GSfxWidget());
      AddWidget((BWidget &) *new GButtonWidget("Credits", GAME_STATE_CREDITS));
      // AddWidget((BWidget &) *new GResetWidget());
      AddWidget((BWidget &) *new GExitWidget());
    }
};

class GMainOptionsProcess : public BProcess {
public:
    GMainOptionsProcess() : BProcess() {
      mContainer = new OptionsContainer(10, WIDGET_CONTAINER_Y);
    }

    ~GMainOptionsProcess() OVERRIDE {
      delete mContainer;
    }

public:
    TBool RunBefore() OVERRIDE {
      return ETrue;
    }

    TBool RunAfter() OVERRIDE {
      mContainer->Render(85, 55);
      mContainer->Run();
      if (gControls.WasPressed(BUTTON_MENU | BUTTON_START)) {
        gGame->SetState(GAME_STATE_MAIN_MENU);
#ifdef ENABLE_AUDIO
        //gSoundPlayer.SfxMenuOut();
#endif
        return EFalse;
      }
      return ETrue;
    }

protected:
    OptionsContainer *mContainer;
    BFont *mFont16;
};

class GMainOptionsPlayfield : public BPlayfield {
public:
    GMainOptionsPlayfield() {
      //Todo : Render Options text instead of "EVADE 2?"
//      gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
//      mBackground = gResourceManager.GetBitmap(BKG_SLOT);
//      gDisplay.SetPalette(mBackground);
      mTitle = new GVectorTitle(60, 70, "OPTIONS");
    }

    ~GMainOptionsPlayfield() OVERRIDE {
//      gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
    }

public:
    void Render() OVERRIDE {
//      gDisplay.renderBitmap->CopyPixels(mBackground);
      mTitle->Render();
    }

    void Animate() {
      mTitle->Move();
    }

public:
    GVectorTitle *mTitle;
//    BBitmap *mBackground;
};


GMainOptionsState::GMainOptionsState() : BGameEngine(gViewPort) {
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  mPlayfield = new GMainOptionsPlayfield();
  AddProcess(new GMainOptionsProcess());

  gWidgetTheme.Configure(
      WIDGET_TEXT_FONT, mFont16,
      WIDGET_TEXT_FG, COLOR_TEXT,
      WIDGET_TEXT_BG, COLOR_TEXT_BG,
      WIDGET_TITLE_FONT, mFont16,
      WIDGET_TITLE_FG, COLOR_TEXT,
      WIDGET_TITLE_BG, -1,
      WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_SLIDER_FG, COLOR_TEXT_BG,
      WIDGET_SLIDER_BG, COLOR_TEXT,
      WIDGET_END_TAG);

  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 255, 92, 93);
}

GMainOptionsState::~GMainOptionsState() {
  delete mFont16;
}


