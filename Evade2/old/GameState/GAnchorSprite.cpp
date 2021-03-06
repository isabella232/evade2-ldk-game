#include <GameState/player/GPlayer.h>
#include <GameState/status/GStatProcess.h>
#include "Game.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"

#if 0
// constructor
GAnchorSprite::GAnchorSprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType)
    : BAnimSprite(aPri, aBM, aImg, aType), mName("NO NAME") {

  mGameState = aGameState;
  SetFlags(SFLAG_ANCHOR | SFLAG_SORTPRI | SFLAG_RENDER_DEBUG);
  if (aType != STYPE_DEFAULT) {
    SetFlags(SFLAG_CHECK);
  }
  mDirection = DIRECTION_DOWN;

  pri = PRIORITY_BELOW;
  mAttributeSave = 0xffff;
  w = 64;
  h = 64;
  mAttackStrength = BASE_STRENGTH;
  mLastX = 0;
  mLastY = 0;
  mCollided = GCollidedData();
  mShadow = TRect();
  mElement = ELEMENT_NONE;
  gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 38, 35, 58);

}

GAnchorSprite::~GAnchorSprite() = default;

void GAnchorSprite::SetMapAttribute(TUint aAttribute) {
  if (mAttributeSave == 0xffff) {
    mAttributeSave = mGameState->mGamePlayfield->GetAttribute(x, y);
  }
  mGameState->mGamePlayfield->SetAttribute(x, y, aAttribute);
}

void GAnchorSprite::ResetMapAttribute() {
  if (mAttributeSave != 0xffff) {
    SetMapAttribute(mAttributeSave);
    mAttributeSave = 0xffff;
  }
}

// safely position sprite so it's not on top of player
void GAnchorSprite::SafePosition(BSprite *aOther) {
  TRect myRect, hisRect;
  aOther->GetRect(hisRect);
  GetRect(myRect);
  if (!myRect.Overlaps(hisRect)) {
    // already safe to position here (not on top of other sprite)
    return;
  }
  x = hisRect.x1 - hisRect.Width() - 1;
  if (CanWalk(0, 0, ETrue)) {
    return;
  }
  x = hisRect.x2 + 1;
}

TBool GAnchorSprite::IsFloorTile(TFloat aX, TFloat aY) {
  return mGameState->mGamePlayfield->IsFloor(aX, aY);
}

TBool GAnchorSprite::CanWalkInDirection(DIRECTION aDirection, TFloat aDx, TFloat aDy) {
  GFloatRect r;
  GetFloatRect(r);
  r.Offset(aDx, aDy);

  switch (aDirection) {
    case DIRECTION_UP:
      for (TInt i = r.x1; i < r.x2; i += WALL_THICKNESS) {
        if (!IsFloorTile(i, r.y1)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y1);
    case DIRECTION_DOWN:
      for (TInt i = r.x1; i < r.x2; i += WALL_THICKNESS) {
        if (!IsFloorTile(i, r.y2)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y2);
    case DIRECTION_LEFT:
      for (TInt i = r.y1; i < r.y2; i += WALL_THICKNESS) {
        if (!IsFloorTile(r.x1, i)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x1, r.y2);
    case DIRECTION_RIGHT:
      for (TInt i = r.y1; i < r.y2; i += WALL_THICKNESS) {
        if (!IsFloorTile(r.x2, i)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y2);
    default:
      return EFalse;
  }
}

TBool GAnchorSprite::CanWalk(TFloat aDx, TFloat aDy, TBool aCheckAllSides) {
  if ((aDy < 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_UP, aDx, aDy)) {
    return EFalse;
  }
  if ((aDy > 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_DOWN, aDx, aDy)) {
    return EFalse;
  }
  if ((aDx < 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_LEFT, aDx, aDy)) {
    return EFalse;
  }
  if ((aDx > 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_RIGHT, aDx, aDy)) {
    return EFalse;
  }

  return ETrue;
}

void GAnchorSprite::Move() {
  mLastX = x;
  mLastY = y;
  BAnimSprite::Move();
  if (!TestFlags(SFLAG_BELOW)) {
    pri = y + 1000;
  }
}

void GAnchorSprite::SaveDataFromCollided(GAnchorSprite *aOther) {
  mCollided.element = aOther->mElement;
  mCollided.direction = aOther->mDirection;
  mCollided.attackStrength = aOther->mAttackStrength;
  if (aOther->TestFlags(SFLAG_KNOCKBACK)) {
    TPoint myCenter = this->Center(), otherCenter = aOther->Center();
    mCollided.collisionAngle = ATAN2(myCenter.x - otherCenter.x, myCenter.y - otherCenter.y) + 2 * M_PI;
    mCollided.vx = aOther->vx;
    mCollided.vy = aOther->vy;
  }
  mCollided.flags = aOther->flags;
}

void GAnchorSprite::Collide(BSprite *aOther) {
  auto *s = (GAnchorSprite *)aOther;
  SaveDataFromCollided(s);
  s->SaveDataFromCollided(this);
  cType |= s->type;
  s->cType |= type;
}

void GAnchorSprite::Nudge() {
  vx = vy = 0;
  x = mLastX;
  y = mLastY;
}

void GAnchorSprite::ResetShadow() {
  mShadow.Set(cx, cy - h, cx + w, cy);
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  TBool ret, overrideClipped = EFalse;
  BBitmap &bm = *gDisplay.renderBitmap;
  if (TestFlags(SFLAG_RENDER_SHADOW) && TestFlags(SFLAG_RENDER)) {
    if (mShadow.x1 == 0 && mShadow.x2 == 0 && mShadow.y1 == 0 && mShadow.y2 == 0) {
      ResetShadow();
    }
    TRect r = mShadow;
    r.Offset(x - aViewPort->mWorldX + aViewPort->mOffsetX, y - aViewPort->mWorldY + aViewPort->mOffsetY);
    if (aViewPort->GetRect().Overlaps(r)) {
      // render shadow beneath sprite
//    gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 40, 40, 60);

      TFloat chord;
      for (TInt i = 0; i < r.Height(); i++) {
        chord = sqrt(i * (r.Height() - i)) * 2 * r.Width() / r.Height();
        bm.DrawFastHLine(aViewPort, r.x2 - aViewPort->mOffsetX - TInt(chord / 2),
                                             r.y1 - aViewPort->mOffsetY + i - 1, chord, COLOR_SHADOW);
      }
      overrideClipped = ETrue;
    }
  }
  RenderAboveShadow(aViewPort);
  ret = BAnimSprite::Render(aViewPort);
  if (overrideClipped) {
    ClearFlags(SFLAG_CLIPPED);
  }

#ifdef DEBUG_MODE
  if (GGame::mDebug && !Clipped() && TestFlags(SFLAG_RENDER_DEBUG)) {
    TPoint p = Center();
    p.Offset(-aViewPort->mWorldX, -aViewPort->mWorldY);
    // render sprite border if sprite is visible
    if (TestFlags(SFLAG_RENDER)) {
      bm.DrawRect(aViewPort, mRect, COLOR_WHITE);
      bm.DrawFastHLine(aViewPort, mRect.x1 - 5, mRect.y2, 10, COLOR_HEALTH);
      bm.DrawFastVLine(aViewPort, mRect.x1, mRect.y2 - 5, 10, COLOR_HEALTH);
//      bm.DrawString(aViewPort, Name(), gFont8x8, mRect.x1 + 1, mRect.y2 + 1, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -1);
      switch (mDirection) {
        case DIRECTION_UP:
          bm.DrawFastVLine(aViewPort, p.x, p.y - 20, 20, COLOR_HEALTH2);
          break;
        case DIRECTION_DOWN:
          bm.DrawFastVLine(aViewPort, p.x, p.y, 20, COLOR_HEALTH2);
          break;
        case DIRECTION_LEFT:
          bm.DrawFastHLine(aViewPort, p.x - 20, p.y, 20, COLOR_HEALTH2);
          break;
        case DIRECTION_RIGHT:
          bm.DrawFastHLine(aViewPort, p.x, p.y, 20, COLOR_HEALTH2);
          break;
        default:
          break;
      }
    }
    // render collision rect
    if (TestFlags(SFLAG_CHECK)) {
      TRect r;
      GetRect(r);
      r.Offset(-aViewPort->mWorldX, -aViewPort->mWorldY);
//      gDisplay.SetColor(COLOR_HEALTH, 255, 0, 0);
      bm.DrawRect(aViewPort, r, COLOR_HEALTH);
    }
    // render velocity vector
    if (vx != 0 || vy != 0) {
      bm.DrawLine(aViewPort, p.x, p.y, p.x + vx * FRAMES_PER_SECOND / 10, p.y + vy * FRAMES_PER_SECOND / 10, COLOR_EXPERIENCE);
    }
  }
#endif

  return ret;
}

DIRECTION GAnchorSprite::RandomDirection() {
  TInt dir = Random() & TUint8(3);
  switch (dir) {
    case 0:
      return DIRECTION_UP;
    case 1:
      return DIRECTION_DOWN;
    case 2:
      return DIRECTION_LEFT;
    case 3:
    default:
      return DIRECTION_RIGHT;
  }
}

DIRECTION GAnchorSprite::RotateDirection(DIRECTION aDirection, TInt aRotateClockwiseCount) {
  aRotateClockwiseCount %= 4;
  if (aRotateClockwiseCount == 0) {
    return aDirection;
  } else {
    // recursive call
    aRotateClockwiseCount--;
    switch (aDirection) {
      case DIRECTION_UP:
        return GAnchorSprite::RotateDirection(DIRECTION_RIGHT, aRotateClockwiseCount);
      case DIRECTION_DOWN:
        return GAnchorSprite::RotateDirection(DIRECTION_LEFT, aRotateClockwiseCount);
      case DIRECTION_LEFT:
        return GAnchorSprite::RotateDirection(DIRECTION_UP, aRotateClockwiseCount);
      case DIRECTION_RIGHT:
        return GAnchorSprite::RotateDirection(DIRECTION_DOWN, aRotateClockwiseCount);
      default:
        return aDirection;
    }
  }
}

DIRECTION GAnchorSprite::VectorToDirection(TInt aDx, TInt aDy) {
  if (aDy < aDx) {
    return aDy < -aDx ? DIRECTION_UP : DIRECTION_RIGHT;
  } else {
    return aDy < -aDx ? DIRECTION_LEFT : DIRECTION_DOWN;
  }
}

void GAnchorSprite::StartAnimationInDirection(ANIMSCRIPT* aScriptGroup[4], DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      StartAnimation(aScriptGroup[DIRECTION_UP]);
      break;
    case DIRECTION_DOWN:
      StartAnimation(aScriptGroup[DIRECTION_DOWN]);
      break;
    case DIRECTION_LEFT:
      StartAnimation(aScriptGroup[DIRECTION_LEFT]);
      break;
    case DIRECTION_RIGHT:
      StartAnimation(aScriptGroup[DIRECTION_RIGHT]);
      break;
    default:
      Panic("No animation direction\n");
      break;
  }
}

void GAnchorSprite::MoveInDirection(TFloat aSpeed, DIRECTION aDirection, TBool aIgnoreWallFlag) {
  TFloat newVx = vx, newVy = vy;
  switch (aDirection) {
    case DIRECTION_UP:
      newVy = -aSpeed;
      break;
    case DIRECTION_DOWN:
      newVy = aSpeed;
      break;
    case DIRECTION_LEFT:
      newVx = -aSpeed;
      break;
    case DIRECTION_RIGHT:
      newVx = aSpeed;
      break;
    default:
      Panic("No move direction\n");
  }
  if (aIgnoreWallFlag || CanWalkInDirection(aDirection, newVx, newVy)) {
    vx = newVx;
    vy = newVy;
  }
}

void GAnchorSprite::GetFloatRect(GFloatRect &aRect) {
  aRect.Set(x + cx + w / 2.0, y + cy - h, x + cx + w + w / 2.0, y + cy);
}

TPoint GAnchorSprite::Center() {
  return TPoint((TInt) x + cx + w, (TInt) y + cy - h / 2);
}

TFloat GAnchorSprite::DistanceTo(GAnchorSprite *aOther) {
  TPoint myCenter = this->Center(),
      otherCenter = aOther->Center();

  return hypot(myCenter.x - otherCenter.x, myCenter.y - otherCenter.y);
}

void GAnchorSprite::WriteToStream(BMemoryStream &aStream) {
  WriteCustomToStream(aStream); // write additional data, if any
  aStream.Write(&x, sizeof(x));
  aStream.Write(&y, sizeof(y));
  aStream.Write(&vx, sizeof(vx));
  aStream.Write(&vy, sizeof(vy));
  aStream.Write(&cx, sizeof(cx));
  aStream.Write(&cy, sizeof(cy));
  aStream.Write(&w, sizeof(w));
  aStream.Write(&h, sizeof(h));
  aStream.Write(&pri, sizeof(pri));
  aStream.Write(&flags, sizeof(flags));
  aStream.Write(&mDirection, sizeof(mDirection));
}

void GAnchorSprite::ReadFromStream(BMemoryStream &aStream) {
  ReadCustomFromStream(aStream);
  aStream.Read(&x, sizeof(x));
  aStream.Read(&y, sizeof(y));
  aStream.Read(&vx, sizeof(vx));
  aStream.Read(&vy, sizeof(vy));
  aStream.Read(&cx, sizeof(cx));
  aStream.Read(&cy, sizeof(cy));
  aStream.Read(&w, sizeof(w));
  aStream.Read(&h, sizeof(h));
  aStream.Read(&pri, sizeof(pri));
  aStream.Read(&flags, sizeof(flags));
  aStream.Read(&mDirection, sizeof(mDirection));
}

void GAnchorSprite::Dump() {
  printf("\nSPRITE @ %p\n", this);
  printf("%-16.16s: %f(%f),%f(%f)\n", "x(vx),y(vy)", x, vx, y, vy);
  printf("%-16.16s: %d,%d,%d,%d\n", "cx,cy,w,h", cx, cy, w, h);
  printf("%-16.16s: %d\n", "pri", pri);
  printf("%-16.16s: %d\n", "flags", flags);
  printf("\n");
}
#endif