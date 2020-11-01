#ifndef GENUS_GMUSICWIDGET_H
#define GENUS_GMUSICWIDGET_H

#include "GSoundSliderWidget.h"
#include "Game.h"
static const TRange music_options = {
    0, 8, 1
};
class GMusicWidget : public GSoundSliderWidget {
public:
  GMusicWidget() : GSoundSliderWidget("Music", &music_options, COLOR_TEXT, COLOR_TEXT_BG) {
    mHeight = 20;
  };
//  ~GMusicWidget() = default;
public:
  TInt Render(TInt aX, TInt aY) OVERRIDE  {
    mSelectedValue = gOptions->music / 0.125;
    return GSoundSliderWidget::Render(aX, aY);
  }
  ;
  void Select(TInt aVal) OVERRIDE  {
    gOptions->music = aVal * 0.125;
    gOptions->muted = EFalse;
    gOptions->Save();
#ifdef ENABLE_AUDIO
    gSoundPlayer.SetMusicVolume(gOptions->music);
    gSoundPlayer.MuteMusic(gOptions->muted);
#endif
  };
};

#endif //GENUS_GMUSICWIDGET_H
