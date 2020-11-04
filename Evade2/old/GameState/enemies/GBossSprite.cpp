#include <Display.h>
#include "GResources.h"
#include "GBossSprite.h"
#include "GPlayer.h"
#include "common/GCamera.h"

static const TFloat z_dist = 256;
static const TFloat frames = 32;

const TInt8 *boss_graphic(TInt16 n) {
  switch (n) {
    case BOSS_1:
      return (const TInt8 *)&boss_1_img;
    case BOSS_2:
      return (const TInt8 *)&boss_2_img;
    case BOSS_3:
      return (const TInt8 *)&boss_3_img;
    default:
      return (const TInt8 *)&boss_4_img;
  }
}



/**
 * Initialize object for Boss 1
 */
static void init_boss_1(GBossSprite *o) {
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
 * Initialize object for Boss 2
 */
static void init_boss_2(GBossSprite *o) {
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
 * Initialize object for Boss 3
 */
static void init_boss_3(GBossSprite *o) {
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
 * Initialize object for Boss 4
 */
static void init_boss_4(GBossSprite *o) {
  o->mColor = SCOUT_COLOR;
  o->mX = Camera::mX + Random(-500, 500);
  o->mY = Camera::mY + Random(-500, 500);
  o->mZ = Camera::mZ + 1024;
  o->mVZ = CAMERA_VZ - 12;
  o->mVX = o->mVY;
  o->mTheta = Random(-50, 50);
  o->mVTheta = Random(-3, 3);
}

GBossSprite::GBossSprite() : GEnemySprite() {

  SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);

  mMeter = 1.0;
}


void GBossSprite::InitEnemyType(TInt aEnemyType) {
  mType = aEnemyType;
  switch (aEnemyType) {
    case BOSS_1:
      printf("InitEnemyType %i BOSS_1\n", aEnemyType);
      mLines = boss_graphic(aEnemyType);
      init_boss_1(this);
      break;
    case BOSS_2:
      printf("InitEnemyType %i BOSS_2\n", aEnemyType);
      mLines = boss_graphic(aEnemyType);
      init_boss_2(this);
      break;
    case BOSS_3:
      printf("InitEnemyType %i BOSS_3\n", aEnemyType);
      mLines = boss_graphic(aEnemyType);
      init_boss_3(this);
      break;
    case BOSS_4:
      printf("InitEnemyType %i BOSS_4\n", aEnemyType);
      mLines = boss_graphic(aEnemyType);
      init_boss_4(this);
      break;
    default:
      Panic("GBossSprite::InitEnemyType Don't know what enemy type to initialize!");
  }
}

//
//GBossSprite::~GBossSprite() {
//
//}

TBool GBossSprite::Render(BViewPort *aViewPort) {
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

void GBossSprite::Move() {
//  mTheta++;
  mTheta += mVTheta;
  mX += mVX;
  mY += mVY;
  mZ += mVZ;
}