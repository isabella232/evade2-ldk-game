#ifndef GENUS_GSTARTWIDGET_H
#define GENUS_GSTARTWIDGET_H

#include "Game.h"

class GStartWidget : public GButtonWidget {
public:
    GStartWidget() : GButtonWidget("New Game", GAME_STATE_GAME)  {

    };
    ~GStartWidget() = default;
};

#endif //GENUS_GSTARTWIDGET_H
