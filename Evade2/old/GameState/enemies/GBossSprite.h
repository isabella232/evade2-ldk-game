#ifndef GBOSS_SPRITE_H
#define GBOSS_SPRITE_H


#include "boss_1_img.h"
#include "boss_2_img.h"
#include "boss_3_img.h"
#include "boss_4_img.h"
#include "bullet_img.h"
#include "ebomb_img.h"
#include "ebullet_img.h"
#include "GEnemySprite.h"

enum {
  BOSS_1,
  BOSS_2,
  BOSS_3,
  BOSS_4,
};



class GBossSprite : public GEnemySprite {
public:
  EXPLICIT GBossSprite();

  ~GBossSprite() = default;
  void Move() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;
  void InitEnemyType(TInt aEnemyType) OVERRIDE;

public:
  TFloat mMeter;
  TInt mType;
  TFloat mVTheta;
};

#endif
