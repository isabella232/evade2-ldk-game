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
  gVectorFont = new GVectorFont();
  mDifficulty = 1;
  gGameEngine = ENull;
  mState      = 0;
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

  mState = aNewState;
  switch (aNewState) {
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
    gGameEngine->GameLoop();
    gDisplay.Update();

    if (gControls.WasPressed(BUTTONQ)) {
      done = ETrue;
    }
  }

#if 0
#ifdef ENABLE_OPTIONS
  TBool muted = gOptions->muted;
#endif


  TBool done = EFalse;
  while (!done) {
    Random(); // randomize
    mShmoo.Set(TUint8(mShmoo.r + 16), TUint8(mShmoo.g + 16), TUint8(mShmoo.b + 16));
    Camera::Move();

    gDisplay.displayBitmap->SetColor(COLOR_SHMOO, mShmoo);
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO_RED, mShmoo.r, 20, 20);
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO_GREEN, 20, mShmoo.g, 20);

    // TODO: Add all the basic colors here
    gDisplay.SetColor(STAR_COLOR, 180, 180, 180);
    gDisplay.SetColor(COLOR_SPACE, 0, 0, 0);

    // The master game loop needs to clear the screen because the star field is ALWAYS running
    gDisplay.renderBitmap->Clear(COLOR_SPACE);

    mStarField->Render();

    if (mNextState != mState) {
      switch (mNextState) {
        case GAME_STATE_SPLASH:
          delete gGameEngine;
          gGameEngine = new GSplashState();
          break;
        case GAME_STATE_MAIN_MENU:
          delete gGameEngine;
          gGameEngine = new GMainMenuState((GGameState *)gGameEngine);
          break;
        case GAME_STATE_MAIN_OPTIONS:
          delete gGameEngine;
          gGameEngine = new GMainOptionsState();
          break;
        case GAME_STATE_RESET_OPTIONS:
          delete gGameEngine;
          gGameEngine = new GResetOptionsState();
          break;
        case GAME_STATE_ATTRACT_MODE:
          delete gGameEngine;
          gGameEngine = new GAttractModeState();
          break;
        case GAME_STATE_RESET_GAME:
        case GAME_STATE_GAME:
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_LOAD_SAVEGAME:
        case GAME_STATE_RESUME_GAME:
          delete gGameEngine;
          gGameEngine = new GGameState((char *)mLocalData);
          break;

        case GAME_STATE_VICTORY:
          delete gGameEngine;
          gGameEngine = new GVictoryState((GGameState *)gGameEngine);
          break;

        case GAME_STATE_CREDITS:
          delete gGameEngine;
          gGameEngine = new GCreditsState((GGameState *)gGameEngine);
          break;

        case GAME_STATE_QUIT:
          done = ETrue;
        default:
          continue;
      }
      // reset dKeys so next state doesn't react to any keys already pressed
      gControls.dKeys = 0;
      mState = mNextState;
    }



    if (mInventory) {
      mInventory->GameLoop();
    }
    else if (mGameMenu) {
      gGameEngine->GameLoop();
      mGameMenu->GameLoop();
    }
    else if (mDebugMenu) {
      gGameEngine->GameLoop();
      mDebugMenu->GameLoop();
    }
    else {
      gGameEngine->GameLoop();
    }

    gDisplay.Update();
    TUint32 now = Milliseconds();
    start = now;

#ifdef FRAME_RATE_INFO
    TUint32 elapsed = now - start;
    printf("elapsed %4d\r", elapsed);
#endif

#ifdef DEBUG_MODE
    if (gControls.WasPressed(CONTROL_DEBUG)) {
      if (IsGameState()) {
        ToggleDebugMenu();
      }
    }
#endif

    if (IsGameState() && gControls.WasPressed(BUTTON_START)) {
      ToggleInGameMenu();
    }

    // right shoulder button brings up inventory
    if (IsGameState() && gControls.WasPressed(CONTROL_INVENTORY)) {
      ToggleInventory();
    }

    if (gControls.WasPressed(BUTTONQ)) {
      done = ETrue;
    }
#ifdef ENABLE_OPTIONS
    if (gControls.WasPressed(BUTTON2)) {
      muted = !muted;
      gOptions->muted = muted;
      gOptions->Save();
#ifdef ENABLE_AUDIO
      //      //gSoundPlayer.MuteMusic(muted);
#endif
    }
#endif
  }
#endif
}
