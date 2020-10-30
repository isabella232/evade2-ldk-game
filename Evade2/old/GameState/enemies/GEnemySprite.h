#ifndef GENEMYSPRITE_H
#define GENEMYSPRITE_H


#include "boss_1_img.h"
#include "boss_2_img.h"
#include "boss_3_img.h"
#include "boss_4_img.h"
#include "bullet_img.h"
#include "ebomb_img.h"
#include "ebullet_img.h"
#include "enemy_assault_1_img.h"
#include "enemy_heavy_bomber_1_img.h"
#include "enemy_scout_1_img.h"
#include "environment_asteroid_img.h"
#include "hud_console_img.h"
#include "radar_blip_img.h"

#include "GVectorSprite.h"


enum {
  ENEMY_SCOUT,
  ENEMY_BOMBER,
  ENEMY_ASSAULT,
};



class GEnemySprite : public GVectorSprite {
public:
  EXPLICIT GEnemySprite();

  ~GEnemySprite() = default;
  void Move() OVERRIDE;

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;
  virtual void InitEnemyType(TInt aEnemyType);

public:
  TFloat mMeter;
  TInt mType;
  TFloat mVTheta;
};

#endif
