#ifndef GENUS_GRESETWIDGET_H
#define GENUS_GRESETWIDGET_H

#include "Game.h"

class GResetWidget : public GButtonWidget {
public:
  GResetWidget()  : GButtonWidget("Reset Game", GAME_STATE_RESET_OPTIONS) {
    mHeight = 20;
  };

//    ~GResetWidget() = default;

  TInt Render(TInt aX, TInt aY) OVERRIDE  {
    GButtonWidget::Render(aX, aY);
    return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
  }
};

#endif //GENUS_GRESETWIDGET_H
