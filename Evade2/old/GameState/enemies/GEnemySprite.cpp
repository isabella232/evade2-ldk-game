#include <Display.h>
#include "GResources.h"
#include "GEnemySprite.h"
#include "GPlayer.h"
#include "GameState/GCamera.h"


const TInt8 *enemy_graphic(TInt16 n) {
  switch (n) {
    case ENEMY_ASSAULT:
      return (const TInt8 *)&enemy_assault_1_img;
    case ENEMY_BOMBER:
      return (const TInt8 *)&enemy_heavy_bomber_1_img;
    default:
      return (const TInt8 *)&enemy_scout_1_img;
  }
}



/**
 * Initialize object for scout enemy
 */
static void init_scout(GEnemySprite *o) {
  o->mColor = SCOUT_COLOR;
  o->mX = Camera::mX + Random(-500, 500);
  o->mY = Camera::mY + Random(-500, 500);
  o->mZ = Camera::mZ + 1024;
  o->mVZ = CAMERA_VZ - 12;
  o->mVX = o->mVY;
  o->mTheta = Random(-50, 50);
  o->mVTheta = Random(-3, 3);
}

/**
 * Initialize Object for assault enemy
 */
static void init_assault(GEnemySprite *o, TBool left) {
  TFloat angle = left ? 0 : (2 * PI);
  o->mColor = ASSAULT_COLOR;
//  o->mX = cos(angle) * 256;
//  o->mZ = Camera::mZ + sin(angle) * 256;
//  o->mY = Camera::mY; //  + 64 - Random(0, 128);
//  o->mVX = o->mVY = o->mVZ = 0;
  o->mX = Camera::mX + Random(-500, 500);
  o->mY = Camera::mY + Random(-500, 500);
  o->mZ = Camera::mZ + 1024;
  o->mVZ = CAMERA_VZ - 12;
  o->mVX = o->mVY;

  o->mTheta = Random(-50, 50);
  o->mVTheta = Random(-3, 3);
  o->mState = 0;
}


/**
 * Initialize object for bomber enemy
 */
static void init_bomber(GEnemySprite *o) {
  o->mColor = BOMBER_COLOR;
//  o->mX = Camera::mX + 128 - Random(0, 127);
//  o->mY = Camera::mY + 128 - Random(0, 127);
//  o->mZ = Camera::mZ - 30;
//  o->mVZ = CAMERA_VZ + 1 ;// TODO: + gGame.Wave();
//  o->mVX = o->mVY = 0;

  o->mX = Camera::mX + Random(-500, 500);
  o->mY = Camera::mY + Random(-500, 500);
  o->mZ = Camera::mZ + 1024;
  o->mVZ = CAMERA_VZ - 12;
  o->mVX = o->mVY;


  o->mTheta = Random(-50, 50);
  o->mVTheta = Random(-3, 3);
}

GEnemySprite::GEnemySprite() : GVectorSprite() {

  SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);

  mMeter = 1.0;
}
void GEnemySprite::InitEnemyType(TInt aEnemyType) {
  mType = aEnemyType;
  switch (aEnemyType) {
    case ENEMY_SCOUT:
      printf("InitEnemyType %i ENEMY_SCOUT\n", aEnemyType);
      mLines = enemy_graphic(aEnemyType);
      init_scout(this);
      break;
    case ENEMY_ASSAULT:
      printf("InitEnemyType %i ENEMY_ASSAULT\n", aEnemyType);
      mLines = enemy_graphic(aEnemyType);
      init_assault(this, EFalse);
      break;
    case ENEMY_BOMBER:
      printf("InitEnemyType %i ENEMY_BOMBER\n", aEnemyType);
      mLines = enemy_graphic(aEnemyType);
      init_bomber(this);
      break;
    default:
      Panic("GEnemySprite::InitEnemyType Don't know what enemy type to initialize!");
  }
}

//
//GEnemySprite::~GEnemySprite() {
//
//}

TBool GEnemySprite::Render(BViewPort *aViewPort) {
//  DrawVectorGraphic(mLines, mX, mY, mTheta, 1, mColor);
//  printf("DrawVector mX=%i, mY=%i, mZ=%i\n", (TInt16)mX, (TInt16)mY, (TInt16)mZ);


  if (!mLines || mZ <= Camera::mZ) {
    printf("DID NOT RENDER mType %i\n", mType);
    // nothing to draw
    return EFalse;
  }

  TFloat zz = (mZ - Camera::mZ) * 2;
  TFloat ratio = 128 / (zz + 128);

  bool isEnemy = mType == OTYPE_ENEMY;
  // printf("is enemey = %i\n", isEnemy);
  TFloat cx = (Camera::mX - mX) * ratio + SCREEN_WIDTH / 2;
  TFloat cy = (Camera::mY - mY) * ratio + SCREEN_HEIGHT / 2;

  return DrawVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, mColor);
}

void GEnemySprite::Move() {
//  mTheta++;
  mTheta += mVTheta;
  mX += mVX;
  mY += mVY;
  mZ += mVZ;
}