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
  gCamera     = new GCamera();
  gVectorFont = new GVectorFont();
  mDifficulty = 1;
  gGameEngine = ENull;
  mState      = 0;
  SetColors();
  mNextState  = GAME_STATE_NONE;
  SetState(GAME_STATE_SPLASH);
  gOptions = new TOptions();

#ifdef ENABLE_AUDIO
  gSoundPlayer.Init(6 /*channels*/);
#endif
  mStarfield = new GStarfield();
//  SetState(GAME_STATE_GAME);
}

GGame::~GGame() {
#ifdef ENABLE_OPTIONS
  delete gOptions;
#endif
  delete gGameEngine;
  delete gViewPort;
  delete mStarfield;
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

void GGame::SetColors() {
  for (TInt i = 0; i < 256; i++) {
    gDisplay.SetColor(i, 255, 255, 255);
  }

  gDisplay.SetColor(COLOR_BLACK, 0, 0, 0);
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);

  gDisplay.SetColor(COLOR_STAR, 255, 255, 255);

  gDisplay.SetColor(ASSAULT_COLOR, 255, 50, 50);
  gDisplay.SetColor(BOMBER_COLOR, 50, 255, 50);
  gDisplay.SetColor(SCOUT_COLOR, 255, 50, 255);
  gDisplay.SetColor(BOSS_COLOR, 102, 102, 255);

  gDisplay.SetColor(EBULLET_COLOR, 50, 50, 255);
  gDisplay.SetColor(BOMB_COLOR, 255, 255, 50);
}

void GGame::SetState(GAMESTATE aNewState) {
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
    mShmoo.Set(TUint8(mShmoo.r + 4), TUint8(mShmoo.g + 4), TUint8(mShmoo.b + 4));
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO, mShmoo);
    gCamera->Move();

    mStarfield->Animate();
    mStarfield->Render();

    if (mNextState != mState) {
      mState = mNextState;
      switch (mNextState) {
        case GAME_STATE_SPLASH:
          delete gGameEngine;
          gGameEngine = new GSplashState();
          break;
        case GAME_STATE_ATTRACT_MODE:
          SetColors();
          delete gGameEngine;
          gGameEngine = new GAttractState();
          break;
        case GAME_STATE_GAME:
          SetColors();
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_MAIN_MENU:
          SetColors();
          delete gGameEngine;
          gGameEngine = new GMainMenuState();
          break;
        case GAME_STATE_VICTORY:
          SetColors();
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_CREDITS:
          SetColors();
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
//    printf(" gCamera vz %2f, z %2f \n", gCamera->vz, gCamera->z);

    if (gControls.WasPressed(BUTTONQ)) {
      done = ETrue;
    }
  }
}
