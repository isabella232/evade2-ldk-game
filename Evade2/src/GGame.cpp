#include "Game.h"
#include "GGame.h"
//#include "GPlayer.h"
#include "common/GCamera.h"
#include "GResources.h"

// states
#include "./GameState/GGameState.h"
#include "./SplashState/GSplashState.h"
#include "./MainMenuState/GMainMenuState.h"
#include "./AttractState/GAttractState.h"

static TUint32 start;

#ifdef DEBUG_MODE
//TBool GGame::mDebug = ETrue;
TBool GGame::mDebug = EFalse;
#else
TBool GGame::mDebug = EFalse;
#endif

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

GGame::GGame() {
  gGame = this;
  printf("Construct GGame\n");
  gCamera = new GCamera();
  gVectorFont = new GVectorFont();
  mDifficulty = 1;
  gGameEngine = ENull;
  mState      = 0;
  mNextState = GAME_STATE_NONE;
  SetState(GAME_STATE_SPLASH);
//  SetState(GAME_STATE_GAME);
}

GGame::~GGame() {
#ifdef ENABLE_OPTIONS
  delete gOptions;
#endif
  delete gGameEngine;
  delete gViewPort;
//  delete mGameMenu;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

//BGameEngine *GGame::CurrentState() {
//  return gGameEngine;
//}


TInt GGame::GetState() const {
  return mState;
}

void GGame::SetState(GAMESTATE aNewState) {
  for (TInt i = 0; i < 256; i++) {
    gDisplay.SetColor(i, 255, 255, 255);
  }

  gDisplay.SetColor(COLOR_BLACK, 0, 0, 0);
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);

  gDisplay.SetColor(COLOR_STAR, 255,255,255);

  gDisplay.SetColor(ASSAULT_COLOR, 255, 50, 50);
  gDisplay.SetColor(BOMBER_COLOR, 50, 255, 50);
  gDisplay.SetColor(SCOUT_COLOR, 255, 50, 255);

  gDisplay.SetColor(EBULLET_COLOR, 50, 50, 255);
  gDisplay.SetColor(BOMB_COLOR, 255, 255, 50);

  mNextState = aNewState;
};

TBool GGame::IsGameState() const {
  return mState == GAME_STATE_GAME;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::Run() {
  TBool done = EFalse;

  while (!done) {
    Random(); // randomize
    mShmoo.Set(TUint8(mShmoo.r + 16), TUint8(mShmoo.g + 16), TUint8(mShmoo.b + 16));
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO, mShmoo);
    gCamera->Move();

    if (mNextState != mState) {
      mState = mNextState;
      switch (mNextState) {
        case GAME_STATE_SPLASH:
          printf("new State SPLASH\n");
          delete gGameEngine;
          gGameEngine = new GSplashState();
          break;
        case GAME_STATE_ATTRACT_MODE:
          printf("new State ATTRACT\n");
          delete gGameEngine;
          gGameEngine = new GAttractState();
          break;
        case GAME_STATE_GAME:
          printf("new State GAME\n");
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_MAIN_MENU:
          printf("new State MAIN MENU\n");
          delete gGameEngine;
          gGameEngine = new GMainMenuState();
          break;
        case GAME_STATE_VICTORY:
          printf("new State VICTORY\n");
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_CREDITS:
          printf("new State CREDITS\n");
          delete gGameEngine;
          gGameEngine = new GAttractState();
          break;
//    case GAME_STATE_NEXT_WAVE:
//      printf("new State NEXT WAVE\n");
//      break;
      }
    }

    gGameEngine->GameLoop();
    gDisplay.Update();

    // TODO: @Jay there is no BUTTONQ on device... maybe we want to exit the game on device by some keys?
    if (gControls.WasPressed(BUTTONQ)) {
      done = ETrue;
    }
  }
}
