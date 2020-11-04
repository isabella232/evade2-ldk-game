#ifndef BRICKOUT_GGAME_H
#define BRICKOUT_GGAME_H

//#include "Game.h"
#include <BApplication.h>
//#include "GStarFieldProcess.h"

class BGameEngine;

class BFont;

enum GAMESTATE {
  GAME_STATE_SPLASH,
  GAME_STATE_MAIN_MENU,
//  GAME_STATE_LOAD_GAME,
//  GAME_STATE_MAIN_OPTIONS,
//  GAME_STATE_RESET_OPTIONS,
  GAME_STATE_ATTRACT_MODE,
//  GAME_STATE_RESET_GAME,
  GAME_STATE_GAME,
//  GAME_STATE_RESUME_GAME,
//  GAME_STATE_LOAD_SAVEGAME,
  GAME_STATE_VICTORY,
  GAME_STATE_CREDITS,
};

class GGame : public BApplication {
public:
  GGame();

  ~GGame() OVERRIDE;

public:
  void Run();

public:
  void SetState(GAMESTATE aNewStae);

  TInt GetState() const;

  TBool IsGameState() const;

public:
  TUint8 mDifficulty;
  TInt16 mKills;
  TInt16 mWave;

  static TBool mDebug;
protected:
  TInt mState;
  TRGB mShmoo;
};

extern GGame       *gGame;
extern BGameEngine *gGameEngine;
extern BFont       *gFont8x8, *gFont16x16;

#endif //BRICKOUT_GGAME_H
