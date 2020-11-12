#include "GVectorSprite.h"
#include "GCamera.h"

void GVectorSprite::SetLines(const TInt8 *aLines) {
  mLines = aLines;
  auto *p = (const TUint16 *) mLines;
  w = p[0];
  h = p[1];
}

TBool GVectorSprite::ExplodeVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                                          TFloat theta, TFloat scaleFactor, TInt8 step, TUint8 color) {
  graphic += 2;
  TBool drawn = false;

  TInt8 numRows = *graphic++;
  TFloat rad = TFloat(theta) * PI / 180, sint = sin(rad), cost = cos(rad);

  for (TInt8 i = 0; i < numRows; i++) {
    struct vec_segment_u8 seg{};
    TFloat                x0, y0, x1, y1;

    memcpy(&seg, graphic, sizeof(seg));
    graphic += sizeof(seg);

    x0 = seg.x0;
    y0 = seg.y0;
    x1 = seg.x1;
    y1 = seg.y1;

    if (scaleFactor) {
      x0 /= scaleFactor;
      y0 /= scaleFactor;
      x1 /= scaleFactor;
      y1 /= scaleFactor;
    }

    if (step) {
      x0 = x0 + ((TFloat)seg.x0 / 8.0f) * (TFloat)step;
      y0 = y0 + ((TFloat)seg.y0 / 8.0f) * (TFloat)step;
      x1 = x1 + ((TFloat)seg.x0 / 8.0f) * (TFloat)step;
      y1 = y1 + ((TFloat)seg.y0 / 8.0f) * (TFloat)step;
    }


    TInt16 drawX1 = x0 * cost - y0 * sint + x,
           drawY1 = y0 * cost + x0 * sint + y,
           drawX2 = x1 * cost - y1 * sint + x,
           drawY2 = y1 * cost + x1 * sint + y;

    TBool xInBounds = (drawX1 >= 0) && (drawX1 <= DISPLAY_WIDTH) && (drawY1 >= 0) && (drawY1 < DISPLAY_HEIGHT),
          yInBounds = (drawX2 >= 0) && (drawX2 <= DISPLAY_WIDTH) && (drawY2 >= 0) && (drawY2 < DISPLAY_HEIGHT);

    if ((!xInBounds) && !(yInBounds)) {
      continue;
    }

    gDisplay.renderBitmap->DrawLine(
      ENull,
      drawX1,
      drawY1,
      drawX2,
      drawY2,
      color
    );
    drawn = true;
  }
  return drawn;
}

static TFloat square(TFloat f) {
  return f * f;
}

TFloat GVectorSprite::DistanceTo(GVectorSprite *aOther) {
  return sqrt(square(aOther->x - x) + square(aOther->y - y) + square(aOther->z - z));
};

TBool GVectorSprite::Render(BViewPort *aViewPort) {
  if (!mLines || z <= gCamera->z) {
    // nothing to draw
    return EFalse;
  }

  TFloat zz    = (z - gCamera->z) * 2;
  TFloat ratio = 128 / (zz + 128);
  auto sw    = TFloat(SCREEN_WIDTH),
       sh    = TFloat(SCREEN_HEIGHT);

  TBool   isEnemy = type == STYPE_ENEMY;
  // printf("is enemy = %i\n", isEnemy);
  TFloat cx      = (gCamera->x - x) * ratio + sw / 2;
  TFloat cy      = (gCamera->y - y) * ratio + sh / 2;

  // uint8_t color = isEnemy ? 5 : 255;

  if (flags & OFLAG_EXPLODE) {
    ExplodeVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, mState, this->mColor);
  } else {
    bool drawn = DrawVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, this->mColor);
    if ((!drawn) && isEnemy) {

      // draw radar blip
      TFloat dx    = gCamera->x - x,
             dy    = gCamera->y - y,
             angle = atan2(dy, dx),
             midx  = sw / 2,
             midy  = sh / 2,
             cxx    = midx + cos(angle) * (midx - 10),
             cyy    = midy + sin(angle) * (midy - 10);

//      printf("Angle: %f, x: %f, y: %f, cx,cy: %f,%f\n", angle, x, y, cx, cy);
      aViewPort->FillCircle(gDisplay.renderBitmap,
                            (TInt16) cxx,
                            (TInt16) cyy,
                            4,
                            this->mColor
//                            mColor
      );

    }
  }

  return ETrue;
}


TBool GVectorSprite::BehindCamera() {
  return z <= gCamera->z;
}