#ifndef GENUS_GSFXWIDGET_H
#define GENUS_GSFXWIDGET_H

#include "GSoundSliderWidget.h"
#include "Game.h"

static const TRange SFX_OPTIONS = {
    0, 8, 1
};

class GSfxWidget : public GSoundSliderWidget {
public:
  GSfxWidget() : GSoundSliderWidget("SFX", &SFX_OPTIONS, COLOR_TEXT, COLOR_TEXT_BG) {
    mHeight = 20;
  }

//  ~GSfxWidget() = default;
public:
  TInt Render(TInt aX, TInt aY) OVERRIDE  {
    mSelectedValue = gOptions->sfx / 0.125;
    return GSoundSliderWidget::Render(aX, aY);
  }

  void Select(TInt aVal) OVERRIDE {
    gOptions->sfx = aVal * 0.125;
    gOptions->muted = EFalse;
    gOptions->Save();

#ifdef ENABLE_AUDIO
  gSoundPlayer.SetEffectsVolume(gOptions->sfx);
  gSoundPlayer.MuteMusic(gOptions->muted);
  gSoundPlayer.SfxOptionSelect();
#endif
  }
};

#endif //GENUS_GSFXWIDGET_H
