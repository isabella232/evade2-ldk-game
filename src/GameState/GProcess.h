#ifndef GPROCESS_H
#define GPROCESS_H

#include <BMemoryStream.h>
#include <BProcess.h>
#include "GVectorSprite.h"
#include "GGamePlayfield.h"

class GProcess : public BProcess {
public:
  EXPLICIT GProcess(TUint16 aAttribute, TInt aPriority = 0) : BProcess(aPriority) {
    mAttribute = aAttribute;
    mSprite = mSprite2 = ENull;
    mSaveToStream = EFalse;
  }

public:
  // factory method
  static GProcess *Spawn(GGameState *aGameState, TInt16 mAttribute, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

public:
  TBool mSaveToStream; // wil save to stream (save game) if true
  TInt16 mAttribute; // type of thing (e.g. ATTR_BAT)
  GVectorSprite *mSprite, *mSprite2;
  GVectorSprite *GetSprite() { return mSprite; }



public:
  virtual void WriteToStream(BMemoryStream &aStream);
  virtual void ReadFromStream(BMemoryStream &aStream);
  // inheritor can override this to write additional instance data to stream
  virtual void WriteCustomToStream(BMemoryStream &aStream) {}
  virtual void ReadCustomFromStream(BMemoryStream &aStream) {}

public:
  virtual void OverlayAnimationComplete(){};
};

#endif

