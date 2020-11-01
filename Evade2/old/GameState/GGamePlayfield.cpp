#include "GGamePlayfield.h"
#include "GResources.h"
#include "GHud.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort) : BPlayfield() {

//  gDisplay.SetPalette(this->mTileset, 0, 128);

  GHud::SetColors();

  TRGB *source = gDisplay.renderBitmap->GetPalette();
  for (TInt color = 0; color < 255; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    gDisplay.SetColor(color, 0,0,0);
  }

}

//GGamePlayfield::~GGamePlayfield() = default;

void GGamePlayfield::Render() {
  BPlayfield::Render();
}

