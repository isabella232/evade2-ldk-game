#include "GBossProcess.h"
#include "GNextWaveProcess.h"
#include "GGameState.h"
#include "GPlayerProcess.h"
#include "GEnemyBulletProcess.h"
#include "GCamera.h"

#include "img/boss_1_img.h"
#include "img/boss_2_img.h"
#include "img/boss_3_img.h"

#define DEBUG_ME 1
#undef DEBUG_ME

static const TFloat z_dist  = 256;
static const TFloat frames  = 64;
//static const TFloat DELTA_X = (DELTA_CONTROL - 1);
static const TFloat DELTA_X = (DELTA_CONTROL / 2);

const TInt TIMER = 240;

enum {
  BOSS_WARP_STATE,
  BOSS_EXPLODE_STATE,
  BOSS_ACTION_STATE,
};

GBossProcess::GBossProcess() : BProcess() {
  mBlink      = 0;
  mColor      = 128;
  mDeltaColor = 1;

  mState  = 0;
  mSprite = new GVectorSprite();
  mSprite->type = STYPE_ENEMY;
  gGameState->AddSprite(mSprite);

  gGameState->mKills = 0;

  // position the boss sprite far out in the distance so we see it grow as we approach during warp in
  gCamera->vx = gCamera->vy = 0;
  mSprite->z  = gCamera->z + z_dist;
  mSprite->vz = gCamera->vz;

  mSprite->mState = 0;

  if (gGameState->mWave % 3 == 0) {
    mType  = 3;
    mLines = boss_3_img;
    mSprite->x      = gCamera->x - 512;
    mSprite->vx     = DELTA_X;
    mSprite->vy     = Random(-3, 3);
    mSprite->mColor = BOSS_COLOR;
    gDisplay.SetColor(BOSS_COLOR, 255, 10, 10);
  } else if (gGameState->mWave % 2 == 0) {
    mType  = 2;
    mLines = boss_2_img;
    mSprite->mColor = BOSS_COLOR;
    gDisplay.SetColor(BOSS_COLOR, 255, 10, 10);
    InitOrbit();
  } else {
    mType  = 1;
    mLines = boss_1_img;
    mSprite->mColor = BOSS_COLOR;
    gDisplay.SetColor(BOSS_COLOR, 255, 10, 10);
    mSprite->x  = gCamera->x + 1024;
    mSprite->vx = -DELTA_X;
    mSprite->y  = gCamera->y;
    mSprite->z  = gCamera->z + z_dist - 150;
  }
  mHitPoints = 20 + (gGame->mDifficulty * mType);
  mSprite->SetLines(mLines);

  mTimer = TIMER;
  gGameState->mState = STATE_WARP;
  SetState(BOSS_WARP_STATE);
  gSoundPlayer.PlayMusic(S11_GET_READY_XM);

}

GBossProcess::~GBossProcess() {
  mSprite->Remove();
  delete mSprite;
}

void GBossProcess::SetState(TInt aNewState) {
  mState = aNewState;

#ifdef DEBUG_ME
  switch (mState) {
    case BOSS_WARP_STATE:
      printf("GBossProcess::mState = BOSS_WARP_STATE\n");
    case BOSS_ACTION_STATE:
      printf("GBossProcess::mState = BOSS_ACTION_STATE\n");
    case BOSS_EXPLODE_STATE:
      printf("GBossProcess::mState = BOSS_EXPLODE_STATE\n");
    default:
      break;
  }
#endif
}

TBool GBossProcess::RunBefore() {
  mColor += mDeltaColor;
  if (mColor > 255) {
    mColor      = 255;
    mDeltaColor = -1;
  } else if (mColor < 128) {
    mColor      = 128;
    mDeltaColor = 1;
  }
  gDisplay.SetColor(BOSS_COLOR, mColor, 0, 0);
  if (mBlink > 0) {
    mBlink--;
    mSprite->SetLines(ENull);
  } else {
    mSprite->SetLines(mLines);
  }
  mBlink = EFalse;
  return ETrue;
}

TBool GBossProcess::Hit() {
  if (mSprite->TestAndClearFlags(OFLAG_COLLISION)) {
//    printf("    hit! %d\n", mHitPoints);
    mHitPoints--;
    mBlink = 240; // off for 4 frames
    gSoundPlayer.TriggerSfx(SFX_BOSS_HIT_WAV, 4);
    return ETrue;
  }
  return EFalse;
}

void GBossProcess::InitOrbit() {
//  printf("InitOrbit\n");
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
    // done with warp
    gCamera->vz        = mSprite->vz = CAMERA_VZ;
    gGameState->mState = STATE_BOSS;
    SetState(BOSS_ACTION_STATE);

    gSoundPlayer.PlayMusic(S11_GET_READY_XM);
    if (gGameState->mWave % 4 == 0) {
      gSoundPlayer.PlayMusic(S08_BOSS_4_XM);
    }
    else if (gGameState->mWave % 3 == 0) {
      gSoundPlayer.PlayMusic(S06_BOSS_3_XM);
    } else if (gGameState->mWave % 2 == 0) {
      gSoundPlayer.PlayMusic(S04_BOSS_2_XM);
    } else {
      gSoundPlayer.PlayMusic(S02_BOSS_1_XM);
    }
    return ETrue;
  }

  gVectorFont->scale = 1.5;
  gVectorFont->printf(90, ALERT_TOP, "WARP TO ACE!");
  gGameState->mPlayerProcess->recharge_shield();
  gGameState->mPlayerProcess->recharge_power();
  return ETrue;
}

TBool GBossProcess::ExplodeState() {
//  printf("ExplodeState\n");
  const TInt16 NUM_FRAMES = 58 * 2;
  mSprite->SetFlags(OFLAG_EXPLODE);
  mSprite->mState++;

  gGameState->mState = STATE_WARP;
  if (mSprite->mState > NUM_FRAMES) {
    gCamera->vz = CAMERA_VZ;

    gGameState->AddProcess(new GNextWaveProcess());
    return EFalse;
  } else {
    return ETrue;
  }
}

void GBossProcess::EngagePlayerRandomXY() {
//  printf("%2f\n", gCamera->z - mSprite->z);

  if ((gCamera->z - mSprite->z) < -300) {
    mSprite->vz = CAMERA_VZ;
  }
  else {
    mSprite->vz = CAMERA_VZ + 2;
  }

  if ((gCamera->z - mSprite->z) >= 0) {
    mSprite->z = gCamera->z + 10;
  }

//  mSprite->z  = gCamera->z + z_dist - 150;

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

  TFloat difficulty = gGame->mDifficulty;
  // Keep within bounds of the screen
  if (mSprite->x - gCamera->x < -300) {
    mSprite->vx = Random(3, DELTA_X + difficulty);
  } else if (mSprite->x - gCamera->x > 300) {
    mSprite->vx = Random(-(DELTA_X + difficulty), -3);
  } else {
    mSprite->vx = Random(-(DELTA_X + difficulty), DELTA_X + difficulty);
  }

  if (mSprite->y - gCamera->y < -300) {
    mSprite->vy = Random(3, DELTA_X + difficulty);
  } else if (mSprite->y - gCamera->y > 300) {
    mSprite->vy = Random(-(DELTA_X + difficulty), -3);
  } else {
    mSprite->vy = Random(-(DELTA_X + difficulty), DELTA_X + difficulty);
  }
}

void GBossProcess::RandomizeFlee() {
//  printf("RandomizeFlee\n");
  mSprite->y      = gCamera->y + Random(-150, 150);
  mSprite->vy     = Random(-7, 7);
  mSprite->vx     = Random(-7, 7);
  mSprite->z      = gCamera->z - 50;
  mSprite->vz     = gCamera->vz + TFloat(Random(1, 7) * gGame->mDifficulty);
  mSprite->mTheta = Random(-180, 180);
}

void GBossProcess::EngagePlayerFlee() {
//  printf("EngagePlayerFlee\n");
  if (mSprite->TestFlags(ORBIT_LEFT)) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->mState = 0;
      RandomizeFlee();
      mSprite->ClearFlags(ORBIT_LEFT);
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 90) {
      mSprite->mState = 90;
      RandomizeFlee();
      mSprite->SetFlags(ORBIT_LEFT);
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
//  printf("EngagePlayerOrbit\n");
  if (mSprite->TestFlags(ORBIT_LEFT)) {
    mSprite->mState -= gGame->mDifficulty;
    if (mSprite->mState < 0) {
      mSprite->y = gCamera->y + Random(-150, 150);
      // mSprite->vy = random(-7,7);

      mSprite->mState = 0;
      mSprite->ClearFlags(ORBIT_LEFT);
    } else {
      mSprite->mTheta -= 6 + (gGameState->mWave * .5);
    }
  } else {
    mSprite->mState += gGame->mDifficulty;
    if (mSprite->mState > 180) {
      mSprite->y      = gCamera->y + Random(-150, 150);
      mSprite->mState = 180;
      mSprite->SetFlags(ORBIT_LEFT);
    } else {
      mSprite->mTheta += 6 + (gGameState->mWave * .5);
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
      mSprite->SetFlags(OFLAG_EXPLODE);
      mSprite->mState = 0;
      mSprite->vz     = gCamera->vz - 3;
      SetState(BOSS_EXPLODE_STATE);
      gSoundPlayer.TriggerSfx(SFX_BOSS_EXPLODE_WAV, 4);
      return ETrue;
    }

    // blink sprite on hit
    mSprite->SetLines(ENull);

    if (mType == 1) {
      mSprite->mState = (mSprite->mState == 1) ? 0 : 1; // mSprite->mState = 1 - mSprite->mState (better!)
    }
  } else {
    // unblink sprite on hit
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
    case BOSS_WARP_STATE:
      return WarpState();
    case BOSS_ACTION_STATE:
      return ActionState();
    case BOSS_EXPLODE_STATE:
      return ExplodeState();
  }
  return ETrue;
}
