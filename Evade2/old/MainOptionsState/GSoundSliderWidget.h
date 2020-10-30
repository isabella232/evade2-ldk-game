#ifndef GENUS_GSOUNDSLIDERWIDGET_H
#define GENUS_GSOUNDSLIDERWIDGET_H

#include "Widgets.h"
#include "Game.h"

class GSoundSliderWidget : public BSliderWidget {
public:
    GSoundSliderWidget(char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground = -1)
       : BSliderWidget(aTitle, aRange, aForeground, aBackground) {
    }

    GSoundSliderWidget(const char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground = -1)
        : BSliderWidget(aTitle, aRange, aForeground, aBackground) {
    }

//    ~GSoundSliderWidget() = default;;
public:
    TInt Render(TInt aX, TInt aY) OVERRIDE {
      aX += 60;
      aY -= 20;

      const BFont  *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
      const TUint8 fg = gWidgetTheme.GetInt(WIDGET_SLIDER_FG),
          bg = gWidgetTheme.GetInt(WIDGET_SLIDER_BG);

      for (TInt i = 0; i < mRange->end; i += mRange->step) {
        gDisplay.renderBitmap->DrawFastVLine(ENull, aX+10, aY - 1, 16, COLOR_TEXT_SHADOW);
        gDisplay.renderBitmap->DrawFastHLine(ENull, aX + 1, aY + 14, 9, COLOR_TEXT_SHADOW);
        gDisplay.renderBitmap->FillRect(ENull, aX, aY - 2, aX + 9, aY + 13, i < mSelectedValue ? fg : bg);
        aX += 10;
      }

      return f->mHeight - 4;
    };

    TInt RenderTitle(TInt aX, TInt aY, TBool aActive = EFalse) OVERRIDE {
      const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

      if (mActive) {
        gDisplay.renderBitmap->DrawStringShadow(
          ENull,
          STR_RIGHT_ARROW,
          f,
          aX - 16, aY,
          gWidgetTheme.GetInt(WIDGET_TEXT_BG),
          COLOR_TEXT_SHADOW,
          COLOR_TEXT_TRANSPARENT
        );
      }

      gDisplay.renderBitmap->DrawStringShadow(
        ENull,
        mTitle,
        f,
        aX, aY,
        gWidgetTheme.GetInt(WIDGET_TITLE_FG),
        COLOR_TEXT_SHADOW,
        COLOR_TEXT_TRANSPARENT,
        -6
      );

      return f->mHeight + 4;
    };

    void Set(TInt aVal) {
      mSelectedValue = aVal;
    };
    void Select(TInt aVal) OVERRIDE = 0;
};

#endif //GENUS_GSOUNDSLIDERWIDGET_H
