#ifndef MODITE_GBOSSPROCESS_H
#define MODITE_GBOSSPROCESS_H

#include "GProcess.h"
#include "GResources.h"
#include "GPlayer.h"
#include "GEnemySprite.h"

class GBossProcess : public GProcess {
public:
    EXPLICIT GBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aAttribute);
    ~GBossProcess() OVERRIDE;

    void SetStatMultipliers(TFloat aModHitPoints = 1.0, TFloat aModStrength = 1.0, TFloat aModExperience = 1.0);

protected:
    void StartBlink(TUint16 aTime) {
      mBlinkTimer = aTime;
    }
    void UpdateBlink();

    TBool BasicDamageCheck();

    void DoDamage(TInt aStrength);

public:
    GEnemySprite *mSprite;
    TInt16 mHitPoints, mMaxHitPoints;

protected:
    GGameState *mGameState;
    TFloat mStartX, mStartY;
    DIRECTION mDirection;
    TUint16 mState, mStep; // state variables

private:
    TUint16 mBlinkTimer;
};

#endif //MODITE_GENEMYPROCESS_H
