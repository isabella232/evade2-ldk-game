#include "GEnemyProcess.h"


const TUint8 BLINK_TIME = 4 * FACTOR;

GEnemyProcess::GEnemyProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute)
    : GProcess(aAttribute), mGameState(aGameState) {


  mSprite = new GEnemySprite();


//  mSprite->Name("ENEMY");
//  mGameState->AddSprite(mSprite);

  mSprite->x = mStartX = aX;
  mSprite->y = mStartY = aY;
  mBlinkTimer = 0;
  mStep = 0;
  mState = 0;

  mHitPoints = mMaxHitPoints = BASE_HIT_POINTS;
}

GEnemyProcess::~GEnemyProcess() {

  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GEnemyProcess::SetStatMultipliers(TFloat aModHitPoints, TFloat aModStrength, TFloat aModExperience) {
//  mHitPoints = mMaxHitPoints = round(aModHitPoints * BASE_HIT_POINTS);
//  mSprite->mAttackStrength = round(aModStrength * BASE_STRENGTH);
}

TBool GEnemyProcess::BasicDamageCheck() {


//  TInt attackAmount = mSprite->mCollided.attackStrength;
//
//  DoDamage(attackAmount);
//  StartBlink(BLINK_TIME);

  return ETrue;
}


void GEnemyProcess::DoDamage(TInt aStrength) {
  // Random +/- 20% damage modifier
  aStrength = (aStrength * Random(80, 120)) / 100;

  if (aStrength > 0) {
    mHitPoints -= aStrength;
//    gSoundPlayer.TriggerSfx(SFX_ENEMY_TAKE_DAMAGE_WAV, 4);
  }

}


void GEnemyProcess::UpdateBlink() {
  if (mBlinkTimer > 1) {
    mBlinkTimer--;
    if ((mBlinkTimer & 1u) == 0) {
      mSprite->mFill = COLOR_WHITE;
    } else {
      mSprite->mFill = -1;
    }
  } else if (mBlinkTimer == 1) {
    mBlinkTimer--;
    mSprite->mFill = -1;
  }
}
