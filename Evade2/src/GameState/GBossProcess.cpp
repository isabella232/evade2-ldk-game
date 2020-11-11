//
// Created by Michael Schwartz on 11/6/20.
//

#include "GBossProcess.h"
#include "GNextWaveProcess.h"
#include "GGameState.h"
#include "GPlayerProcess.h"
#include "GEnemyBulletProcess.h"
#include "GCamera.h"

#include "img/boss_1_img.h"
#include "img/boss_2_img.h"
#include "img/boss_3_img.h"

static const TFloat z_dist = 256;
static const TFloat frames = 32;

const TInt TIMER = 240;

enum {
  WARP_STATE,
  EXPLODE_STATE,
  ACTION_STATE,
};

GBossProcess::GBossProcess() : BProcess() {
  mSprite = new GVectorSprite();
  mSprite->z      = gCamera->z + TIMER * 30 + 512;
  mSprite->x      = gCamera->x;
  mSprite->y      = gCamera->y;
  mSprite->vx     = mSprite->vy = mSprite->vz = 0;
  mSprite->mState = 0;

  gGameState->AddSprite(mSprite);
  mTimer = TIMER;
  mState = WARP_STATE;
  gGameState->mState = STATE_BOSS;

  if (gGameState->mWave % 3 == 0) {
    mType  = 3;
    mLines = boss_3_img;
    mSprite->SetLines(boss_3_img);
    mSprite->x  = gCamera->x - 512;
    mSprite->vx = 10;
    mSprite->vy = Random(-3, 3);
  } else if (gGameState->mWave % 2 == 0) {
    mType  = 2;
    mLines = boss_2_img;
    mSprite->SetLines(boss_2_img);
    InitOrbit();
  } else {
    mType  = 1;
    mLines = boss_1_img;
    mSprite->SetLines(boss_1_img);
    mSprite->x  = gCamera->x + 512;
    mSprite->vx = -10;
    mSprite->y  = gCamera->y;
  }
  mHitPoints = 20 + (gGame->mDifficulty * mType);
}

GBossProcess::~GBossProcess() {
  mSprite->Remove();
  delete mSprite;
}

TBool GBossProcess::RunBefore() {
  if (gGameState->mState != STATE_PLAY) {
    return ETrue;
  }
  return ETrue;
}

TBool GBossProcess::Hit() {
  if (mSprite->flags & OFLAG_COLLISION) {
    mHitPoints--;
    mSprite->flags &= ~OFLAG_COLLISION;
    return ETrue;
  }
  return EFalse;
}

void GBossProcess::InitOrbit() {
  TBool  left  = Random() & 1;
  TFloat angle = left ? 0 : (2 * PI);
  mSprite->x      = cos(angle) * 256;
  mSprite->z      = gCamera->z + sin(angle) * 256;
  mSprite->y      = gCamera->y + Random(30, 90);
  mSprite->vy     = Random(-6 + (gGame->mDifficulty * -1), 6 + (gGame->mDifficulty));
  mSprite->vx     = 0;
  mSprite->vz     = -50 - (gGame->mDifficulty * 2);
  mSprite->mState = left ? 0 : 180;
}

TBool GBossProcess::WarpState() {
  if (mTimer-- < 0) {
    mState = ACTION_STATE;
    gGameState->mState = STATE_BOSS;
    gCamera->vz        = CAMERA_VZ;
    mSprite->vz        = CAMERA_VZ;
    return ETrue;
  }

  gVectorFont->scale = 1.5;
  gCamera->vz        = CAMERA_VZ;
  gVectorFont->printf(90, ALERT_TOP, "WARP TO ACE!");
  gGameState->mPlayerProcess->recharge_shield();
  gGameState->mPlayerProcess->recharge_power();
  return ETrue;
}

TBool GBossProcess::ExplodeState() {
  const TInt16 NUM_FRAMES = 58 * 2;
  mSprite->flags |= OFLAG_EXPLODE;
  mSprite->mState++;

//  EBullet::genocide(); // Kill all enemy bullets
  // Done exploding, move forward to the next wave
  if (mSprite->mState > NUM_FRAMES) {
    mState = STATE_NEXT_WAVE;
    gCamera->vz = CAMERA_VZ;

    gGameState->AddProcess(new GNextWaveProcess());
    return EFalse;
  } else {
    return ETrue;
  }
}

void GBossProcess::EngagePlayerRandomXY() {
  mSprite->z = gCamera->z + z_dist - 150;

  // Debugging stuff
  // Font::scale = .7 * 256;
  // Font::printf(5, 5, "%f", mSprite->x - gCamera->x);
  // Font::printf(5, 15, "%f", mSprite->y - gCamera->y);

  if (mSprite->mState == 1) {
    mSprite->mTheta += 5. + gGame->mDifficulty;
  } else {
    mSprite->mTheta -= 5. + gGame->mDifficulty;
  }
  // Debug
  // mSprite->x = gCamera->x;
  // mSprite->y = gCamera->y;

  if (--mSprite->mTimer > 0) {
    return;
  }

  gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));
  mSprite->mTimer = gGameState->mWave > 20 ? 10 : (40 - gGame->mDifficulty);
  // Keep within bounds of the screen
  if (mSprite->x - gCamera->x < -300) {
    mSprite->vx = Random(3, 10 + gGame->mDifficulty);
  } else if (mSprite->x - gCamera->x > 300) {
    mSprite->vx = Random(-3, -10 + gGame->mDifficulty * -1);
  } else {
    mSprite->vx = Random(-10 + (gGame->mDifficulty * -1), 10 + gGame->mDifficulty);
  }

  if (mSprite->y - gCamera->y < -300) {
    mSprite->vy = Random(3, 10 + gGame->mDifficulty);
  } else if (mSprite->y - gCamera->y > 300) {
    mSprite->vy = Random(-3, -10 + gGame->mDifficulty * -1);
  } else {
    mSprite->vy = Random(-10 + (gGame->mDifficulty * -1), 10 + gGame->mDifficulty);
  }
}

void GBossProcess::RandomizeFlee() {
  mSprite->y      = gCamera->y + Random(-150, 150);
  mSprite->vy     = Random(-7, 7);
  mSprite->vx     = Random(-7, 7);
  mSprite->z      = gCamera->z - 50;
  mSprite->vz     = gCamera->vz + TFloat(Random(1, 7) * gGame->mDifficulty);
  mSprite->mTheta = Random(-180, 180);
}

void GBossProcess::EngagePlayerFlee() {
  if (mSprite->flags & ORBIT_LEFT) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->mState = 0;
      RandomizeFlee();
      mSprite->flags &= ~ORBIT_LEFT;
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 90) {
      mSprite->mState = 90;
      RandomizeFlee();
      mSprite->flags |= ORBIT_LEFT;
    }
  }

  if (--mSprite->mTimer > 0) {
    return;
  }
  gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));

  mSprite->mTimer = gGameState->mWave > 20 ? 20 : (50 - gGame->mDifficulty);
  // mSprite->x = gCamera->x;
  // mSprite->y = gCamera->y;
  mSprite->vx += Random(-7, 7);
  mSprite->vy += Random(-7, 7);
}

void GBossProcess::EngagePlayerOrbit() {
  if (mSprite->flags & ORBIT_LEFT) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->y = gCamera->y + Random(-150, 150);
      // mSprite->vy = random(-7,7);

      mSprite->mState = 0;
      mSprite->flags &= ~ORBIT_LEFT;
    } else {
      mSprite->mTheta -= 12;
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 180) {
      mSprite->y      = gCamera->y + Random(-150, 150);
      mSprite->mState = 180;
      mSprite->flags |= ORBIT_LEFT;
    } else {
      mSprite->mTheta += 12;
    }
  }

  TFloat rad = RADIANS(mSprite->mState);
  mSprite->x = cos(rad) * 512;
  mSprite->z = gCamera->z + sin(rad) * 512;

  if (--mSprite->mTimer <= 0) {
    mSprite->mTimer = gGameState->mWave > 20 ? 20 : (50 - gGame->mDifficulty);
    gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));
  }
}

TBool GBossProcess::ActionState() {
  if (Hit()) {
    if (mHitPoints <= 2) {
      mSprite->flags &= OFLAG_EXPLODE;
      mSprite->mState = 0;
      mSprite->vz     = gCamera->vz - 3;

//      Sound::play_sound(SFX_BOSS_EXPLODE);
      mState = EXPLODE_STATE;
      return ETrue;
    }

    mSprite->SetLines(ENull);

    if (mType == 1) {
      // mSprite->y = Random(-5, 5);
      mSprite->mState = (mSprite->mState == 1) ? 0 : 1;
    }
    // else if (Boss::boss_type == 2) {
    // init_orbit(o, Random() & 1);
    // }
    // else {
    //   randomize_flee(o);
    // }
  } else {
    mSprite->SetLines(mLines);

    if (mType == 1) {
      EngagePlayerRandomXY();
    } else if (mType == 2) {
      EngagePlayerOrbit();
    } else {
      EngagePlayerFlee();
    }
  }

  return ETrue;
}


TBool GBossProcess::RunAfter() {
  switch (mState) {
    case WARP_STATE:
      return WarpState();
    case EXPLODE_STATE:
      return ExplodeState();
  }
  return ETrue;

//  if (mTimer-- < 0) {
//    gGameState->mState = STATE_BOSS;
//    gCamera->vz        = CAMERA_VZ;
//    mSprite->vz        = CAMERA_VZ;
//  } else {
//  }
//  return ETrue;
}

#if 0
TUint16 Boss::hit_points = 0;
UTInt8  Boss::boss_type;

static BOOL hit(Object *o) {
  if (mSprite->flags & OFLAG_COLLISION) {
    Boss::hit_points--;
    mSprite->flags &= ~OFLAG_COLLISION;
    return TRUE;
  }
  return FALSE;
}

const TInt8 *getBossLines() {
  switch (Boss::boss_type) {
    case 3:
      return boss_3_img;
      break;
    case 2:
      return boss_2_img;
      break;
    default:
      return boss_1_img;
  }
}

/**
Ideas:
instead of randomizing vx, vy, you can set y to sin(theta)*64 or something like
that (edited) [18:43] and change theta over time [18:43] it'll make it a
sinusoidal pattern
*/

static void engage_player_random_xy(Object *o) {
  mSprite->z = gCamera->z + z_dist - 150;

  // Debugging stuff
  // Font::scale = .7 * 256;
  // Font::printf(5, 5, "%f", mSprite->x - gCamera->x);
  // Font::printf(5, 15, "%f", mSprite->y - gCamera->y);

  if (mSprite->mState == 1) {
    mSprite->theta += 5 + gGame->mDifficulty;
  } else {
    mSprite->theta -= 5 + gGame->mDifficulty;
  }
  // Debug
  // mSprite->x = gCamera->x;
  // mSprite->y = gCamera->y;

  if (--mSprite->timer > 0) {
    return;
  }
  gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));
  mSprite->timer = gGame->mWave > 20 ? 10 : (40 - gGame->mDifficulty);
  // Keep within bounds of the screen
  if (mSprite->x - gCamera->x < -300) {
    mSprite->vx = random(3, 10 + gGame->mDifficulty);
  } else if (mSprite->x - gCamera->x > 300) {
    mSprite->vx = random(-3, -10 + gGame->mDifficulty * -1);
  } else {
    mSprite->vx = random(-10 + (gGame->mDifficulty * -1), 10 + gGame->mDifficulty);
  }

  if (mSprite->y - gCamera->y < -300) {
    mSprite->vy = random(3, 10 + gGame->mDifficulty);
  } else if (mSprite->y - gCamera->y > 300) {
    mSprite->vy = random(-3, -10 + gGame->mDifficulty * -1);
  } else {
    mSprite->vy = random(-10 + (gGame->mDifficulty * -1), 10 + gGame->mDifficulty);
  }
}

static void randomize_flee(Object *o) {
  mSprite->y     = gCamera->y + random(-150, 150);
  mSprite->vy    = random(-7, 7);
  mSprite->vx    = random(-7, 7);
  mSprite->z     = gCamera->z - 50;
  mSprite->vz    = gCamera->vz + (random(1, 7) * gGame->mDifficulty);
  mSprite->theta = random(-180, 180);
}

static void engage_player_flee(Object *o) {

  if (mSprite->flags & ORBIT_LEFT) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->mState = 0;
      randomize_flee(o);
      mSprite->flags &= ~ORBIT_LEFT;
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 90) {
      mSprite->mState = 90;
      randomize_flee(o);
      mSprite->flags |= ORBIT_LEFT;
    }
  }

  if (--mSprite->timer > 0) {
    return;
  }
  gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));

  mSprite->timer = gGame->mWave > 20 ? 20 : (50 - gGame->mDifficulty);
  // mSprite->x = gCamera->x;
  // mSprite->y = gCamera->y;
  mSprite->vx += random(-7, 7);
  mSprite->vy += random(-7, 7);
}

// Copy of init_assault
static void init_orbit(Object *o, BOOL left) {
  TFloat angle = left ? 0 : (2 * PI);
  mSprite->x      = cos(angle) * 256;
  mSprite->z      = gCamera->z + sin(angle) * 256;
  mSprite->y      = gCamera->y + random(30, 90);
  mSprite->vy     = random(-6 + (gGame->mDifficulty * -1), 6 + (gGame->mDifficulty));
  mSprite->vx     = 0;
  mSprite->vz     = -50 - (gGame->mDifficulty * 2);
  mSprite->mState = left ? 0 : 180;
}

static void engage_player_orbit(Object *o) {

  if (mSprite->flags & ORBIT_LEFT) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->y = gCamera->y + random(-150, 150);
      // mSprite->vy = random(-7,7);

      mSprite->mState = 0;
      mSprite->flags &= ~ORBIT_LEFT;
    } else {
      mSprite->theta -= 12;
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 180) {
      mSprite->y      = gCamera->y + random(-150, 150);
      mSprite->mState = 180;
      mSprite->flags |= ORBIT_LEFT;
    } else {
      mSprite->theta += 12;
    }
  }

  TFloat rad = RADIANS(mSprite->mState);
  mSprite->x = cos(rad) * 512;
  mSprite->z = gCamera->z + sin(rad) * 512;

  if (--mSprite->timer <= 0) {
    mSprite->timer = gGame->mWave > 20 ? 20 : (50 - gGame->mDifficulty);
    gGameState->AddProcess(new GEnemyBulletProcess(mSprite, EBULLET_BOMB));
  }
}

/**
 * Boss is exploding state.
 */
void Boss::explode(Process *me, Object *o) {
  const WORD NUM_FRAMES = 58;
  mSprite->flags |= OFLAG_EXPLODE;
  mSprite->mState++;
  EBullet::genocide(); // Kill all enemy bullets
  // Done exploding, move forward to the next wave
  if (mSprite->mState > NUM_FRAMES) {
    game_mode   = MODE_NEXT_WAVE;
    Game::kills = 65;
    gCamera->vz = CAMERA_VZ;
    Sound::play_score(NEXT_WAVE_SONG);

    ProcessManager::birth(Game::next_wave);
    me->suicide();
  } else {
    me->sleep(1, explode);
  }
}

void Boss::action(Process *me, Object *o) {
  if (hit(o)) {
    if (Boss::hit_points <= 2) {

      mSprite->flags &= OFLAG_EXPLODE;
      mSprite->mState = 0;
      mSprite->vz     = gCamera->vz - 3;

      Sound::play_sound(SFX_BOSS_EXPLODE);
      me->sleep(1, explode);
      return;
    }

    mSprite->lines = NULL;

    if (Boss::boss_type == 1) {
      // mSprite->y = random(-5, 5);
      mSprite->mState = (mSprite->mState == 1) ? 0 : 1;
    }
    // else if (Boss::boss_type == 2) {
    // init_orbit(o, random() & 1);
    // }
    // else {
    //   randomize_flee(o);
    // }
  } else {
    mSprite->lines = getBossLines();

    if (Boss::boss_type == 1) {
      engage_player_random_xy(o);
    } else if (Boss::boss_type == 2) {
      engage_player_orbit(o);
    } else {
      engage_player_flee(o);
    }
  }

  me->sleep(1);
}

void Boss::start_action(Process *me, Object *o) {

  if (Boss::boss_type == 2) {
    if (--mSprite->timer > 0) {
      game_mode = MODE_GAME;
      me->sleep(1, action);
    } else {
      me->sleep(1);
    }
  } else {
    mSprite->y = gCamera->y;
    mSprite->z = gCamera->z + z_dist;
    if (Boss::boss_type == 1) {
      mSprite->z = gCamera->z + z_dist - 150;
      if (mSprite->x <= gCamera->x) {
        game_mode = MODE_GAME;
        me->sleep(1, action);
      } else {
        me->sleep(1);
      }
    } else {
      if (mSprite->x > gCamera->x) {
        game_mode = MODE_GAME;
        me->sleep(1, action);
      } else {
        me->sleep(1);
      }
    }
  }
}

void Boss::entry(Process *me, Object *o) {
  // production
  game_mode   = MODE_NEXT_WAVE;
  Game::kills = 0;
  gCamera->vz = -CAMERA_VZ;

  mSprite->set_type(OTYPE_ENEMY);
  mSprite->z = gCamera->z + z_dist - 150;

  mSprite->mState = 0;
  mSprite->vz     = gCamera->vz;
  mSprite->color  = BOSS_COLOR;

  if (gGame->mWave % 3 == 0) {
    Boss::boss_type = 3;
    mSprite->x  = gCamera->x - 512;
    mSprite->vx = +10;
    mSprite->vy = random(-3, 3);
    Sound::play_score(STAGE_3_BOSS_SONG);
  } else if (gGame->mWave % 2 == 0) {
    Boss::boss_type = 2;
    init_orbit(o, random() & 1);
    Sound::play_score(STAGE_2_BOSS_SONG);
  } else {
    Boss::boss_type = 1;
    mSprite->x  = gCamera->x + 512;
    mSprite->vx = -10;
    mSprite->y  = gCamera->y;
    Sound::play_score(STAGE_1_BOSS_SONG);
  }

  mSprite->lines = getBossLines();

  // PRODUCTION
  Boss::hit_points = 20 + (gGame->mDifficulty * Boss::boss_type);
  // DEV/TEST
  // Boss::hit_points = 1;
  me->sleep(1, Boss::start_action);
}


#endif