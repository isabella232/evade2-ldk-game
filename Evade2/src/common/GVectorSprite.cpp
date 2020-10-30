#include "GVectorSprite.h"
#include "GCamera.h"


TBool GVectorSprite::ExplodeVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                           TFloat theta, TFloat scaleFactor, TInt8 step, TUint8 color) {
  graphic += 2;
  TBool drawn = false;

  TInt8 numRows = *graphic++;

  TFloat rad = TFloat(theta) * PI / 180, sint = sin(rad), cost = cos(rad);

  for (TInt8 i = 0; i < numRows; i++) {
    struct vec_segment_u8 seg;
    TFloat x0, y0, x1, y1;

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
      x0 = x0 + (seg.x0 / 8) * step;
      y0 = y0 + (seg.y0 / 8) * step;
      x1 = x1 + (seg.x0 / 8) * step;
      y1 = y1 + (seg.y0 / 8) * step;
    }



    TInt16 drawX1 = x0 * cost - y0 * sint + x,
        drawY1 = y0 * cost + x0 * sint + y,
        drawX2 = x1 * cost - y1 * sint + x,
        drawY2 = y1 * cost + x1 * sint + y;

    TBool xInBounds = (drawX1 >= 0) && (drawX1 <= DISPLAY_WIDTH) && (drawY1 >= 0) && (drawY1 < DISPLAY_HEIGHT),
        yInBounds = (drawX2 >= 0) && (drawX2 <= DISPLAY_WIDTH) && (drawY2 >= 0) && (drawY2 < DISPLAY_HEIGHT);

    if ((! xInBounds) && !(yInBounds)) {
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

TBool GVectorSprite::Render(BViewPort *aViewPort) {
  if (!mLines || mZ <= GCamera::mZ) {
    // nothing to draw
    return EFalse;
  }

  TFloat zz = (mZ - GCamera::mZ) * 2;
  TFloat ratio = 128 / (zz + 128);

  bool isEnemy = Type() == OTYPE_ENEMY;
  // printf("is enemy = %i\n", isEnemy);
  TFloat cx = (GCamera::mX - mX) * ratio + SCREEN_WIDTH / 2;
  TFloat cy = (GCamera::mY - mY) * ratio + SCREEN_HEIGHT / 2;

  // uint8_t color = isEnemy ? 5 : 255;

  if (flags & OFLAG_EXPLODE) {
    ExplodeVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, mState, this->mColor);
  }
  else {
    bool drawn = DrawVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, this->mColor);
    if ((!drawn) && isEnemy) {

      // draw radar blip
      TFloat dx = GCamera::mX - mX,
          dy = GCamera::mY - mY,
          angle = atan2(dy, dx);

//          printf("TODO: Fill Circle for enemy radar\n");
        aViewPort->FillCircle(gDisplay.renderBitmap,
            (int16_t)(SCREEN_WIDTH / 2 + cos(angle) * 75),
            (int16_t)(SCREEN_HEIGHT / 2 + sin(angle) * 75),
            3,
            EBULLET_COLOR
        );

    }
  }

  return ETrue;
}


TBool GVectorSprite::BehindCamera()  {
  return mZ <= GCamera::mZ;
}