#ifndef GENUS_GEXITWIDGET_H
#define GENUS_GEXITWIDGET_H

#include "Game.h"

class GExitWidget : public GButtonWidget {
public:
    GExitWidget() : GButtonWidget("Back") {
      mHeight = 20;
    };
//    ~GExitWidget() = default;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE  {
      // aY += 18;
      GButtonWidget::Render(aX, aY);
      return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
    };
    void Select() OVERRIDE {
      gGame->SetState(GAME_STATE_MAIN_MENU);
#ifdef ENABLE_AUDIO
      //gSoundPlayer.SfxMenuOut();
#endif
    }

};

#endif //GENUS_GEXITWIDGET_H
