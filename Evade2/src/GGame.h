#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

#include <BApplication.h>
#include "GStarfield.h"
class BGameEngine;

class BFont;

enum GAMESTATE {
  GAME_STATE_NONE,
  GAME_STATE_SPLASH,
  GAME_STATE_MAIN_MENU,
  GAME_STATE_ATTRACT_MODE,
  GAME_STATE_GAME,
//  GAME_STATE_NEXT_WAVE,
  GAME_STATE_VICTORY,
  GAME_STATE_CREDITS,
};

class GGame : public BApplication {
public:
  GGame();

  ~GGame() OVERRIDE;

public:
  void Run();

protected:
  static void SetColors();

public:
  void SetState(GAMESTATE aNewState);

  TInt GetState() const;

  TBool IsGameState() const;

  TUint8       mDifficulty;
  static TBool mDebug;
  GStarfield *mStarfield;

protected:
  TInt mNextState;
  TInt mState;
  TRGB mShmoo;
};

extern GGame       *gGame;
extern BGameEngine *gGameEngine;
extern BFont       *gFont8x8, *gFont16x16;

#endif //GGame.h
