#include "GAttractModeState.h"
#include "Game.h"
#include "Font.h"


static const TInt8 TYPEWRITER_SPEED = 3;
static const TInt8 LINE_HEIGHT = 26;

static const char scout_text[] = "SCOUT";
static const char bomber_text[] = "BOMBER";
static const char assault_text[] = "ASSAULT";

static const char credits1[] = "CRAFTED BY:\nMODUS CREATE\nDECEMBER 2017";
static const char credits2[] = "MUSIC and SFX:\nJ. GARCIA";
static const char credits3[] =
    "ART:\nM. TINTIUC\nJV DALEN\nJD JONES\nJ. GARCIA";
static const char credits4[] =
    "PROGRAMMING:\nM. SCHWARTZ\nJ. GARCIA\nM. TINTIUC\n";
static const char credits5[] =
    "PROGRAMMING:\nD. BRIGNOLI\nS. LEMMONS\nA. DENNIS";
static const char credits6[] = "PROGRAMMING:\nV. POPA\nL. STILL\nG. GRISOGONO";

const TInt8 MAX_SCREEN = 2;
const TInt8 MAX_CREDITS = 5;

struct attract_data {
    TInt8 screen;
    TInt8 x;
    TInt8 y;
    TInt8 offset;
    TInt16 timer;
    TBool done;
    const char* text;
    TInt8 enemy;
};



GAttractModeState::GAttractModeState() : BGameEngine(gViewPort) {

  mEnemySprite = new GEnemySprite();
  mEnemySprite->InitEnemyType(ENEMY_ASSAULT);

//  mEnemySprite->mVX = +5;
  mEnemySprite->mVY = 0;
  mEnemySprite->mVZ = 0;



//  mEnemySprite->mX = -550;
//  mEnemySprite->mY = -450;
  mEnemySprite->mVZ = CAMERA_VZ - 5;

//  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
//  gDisplay.SetColor(COLOR_TEXT_BG, 255, 92, 93);
//  gDisplay.SetColor(COLOR_TEXT_SHADOW, 255, 0, 0);
}

GAttractModeState::~GAttractModeState() {
}

void GAttractModeState::PreRender() {
  if (gControls.WasPressed(BUTTON_ANY)) {
    gGame->SetState(GAME_STATE_MAIN_MENU);
  }
//  mContainer->Run();
}

void GAttractModeState::PostRender() {
  mEnemySprite->Move();
  if (! mEnemySprite->Render(gViewPort)) {
    switch (mEnemySprite->mType) {
      case ENEMY_SCOUT:
        mEnemySprite->InitEnemyType(ENEMY_ASSAULT);
        break;
      case ENEMY_ASSAULT:
        mEnemySprite->InitEnemyType(ENEMY_BOMBER);
        break;
      case ENEMY_BOMBER:
        mEnemySprite->InitEnemyType(ENEMY_SCOUT);
        break;

    }
//  mEnemySprite->mVX = +5;
    mEnemySprite->mVY = 0;
    mEnemySprite->mVZ = 0;
//  mEnemySprite->mX = -550;
//  mEnemySprite->mY = -450;
    mEnemySprite->mVZ = CAMERA_VZ - 16;




  }
//  mContainer->Render(30, 10);
//  printf("GAttractModeState::PostRender()\n");
}

//
//
//static void init_screen(attract_data *ad, TInt16 x = 20, TInt16 y = 30) {
////  if (game_mode == MODE_ATTRACT) {
//  switch (ad->screen) {
////    case 0:
////      ad->enemy = ENEMY_SCOUT;
////      ad->text = scout_text;
////      x = 125;
////      y = 60;
////      break;
////    case 1:
////      ad->enemy = ENEMY_BOMBER;
////      ad->text = bomber_text;
////      x = 117;
////      y = 60;
////      break;
////    case 2:
////      ad->enemy = ENEMY_ASSAULT;
////      ad->text = assault_text;
////      x = 110;
////      y = 60;
////      break;
//  }
////  }
////  else {
////    ad->enemy = -1;
////    switch (ad->screen) {
////      case 0:
////        ad->text = credits1;
////        break;
////      case 1:
////        ad->text = credits2;
////        break;
////      case 2:
////        ad->text = credits3;
////        break;
////      case 3:
////        ad->text = credits4;
////        break;
////      case 4:
////        ad->text = credits5;
////        break;
////      case 5:
////        ad->text = credits6;
////        break;
////    }
////  }
//
//  ad->offset = 1;
//  ad->x = x;
//  ad->y = y;
//  ad->timer = TYPEWRITER_SPEED;
//  ad->done = EFalse;
//}
//
//void GAttractModeState::next(GAttractModeState *me, Object *o) {
//  attract_data *ad = (attract_data *)&o->x;
//
//  ad->timer--;
//  if (ad->timer < 0) {
//    ad->screen++;
////    if ((game_mode == MODE_ATTRACT && ad->screen > MAX_SCREEN) ||
////        (game_mode == MODE_CREDITS && ad->screen > MAX_CREDITS)) {
////      ProcessManager::birth(Splash::entry);
////      me->suicide();
////      return;
////    }
////    else {
//      gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_SCREEN_WAV);
//      init_screen(ad);
////      me->sleep(1, typewriter);
////    }
//  }
////  else {
////    me->sleep(1);
////  }
//}

//
//void GAttractModeState::typewriter(Process *me, Object *o) {
//  attract_data *ad = (attract_data *)&o->x;
//
//  ad->timer--;
//
//  if (gControls.WasPressed(BUTTONA)) {
//    gGame->SetState(GAME_STATE_MAIN_MENU);
////    me->suicide();
//  }
//  if (gControls.WasPressed(BUTTON_START)) {
//    ad->timer = -1;
////    me->sleep(1, next);
//    return;
//  }
//
//  if (ad->timer < 0) {
//    if (ad->done) {
//      ad->timer = 5;
////      me->sleep(1, next);
//      return;
//    }
//    ad->timer = TYPEWRITER_SPEED;
//    ad->offset++;
//    gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_CHAR_WAV);
//  }
//
//  if (ad->enemy != -1) {
//    Graphics::drawVectorGraphic(Enemy::enemy_graphic(ad->enemy),
//                                SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 0.0,
//                                .75, // Originally 2.0
//                                COLOR_WHITE);
//  }
////  if (game_mode == MODE_CREDITS) {
////    Font::scale = 2 * 256;
////  }
//
//  const char* p = ad->text;
//  TInt16 x = ad->x, y = ad->y;
//
//  for (TInt8 i = 0; i < ad->offset;) {
//    char c = *p++;
//    if (c == '\0') {
//      if (!ad->done) {
//        ad->timer = 60; // 2 seconds
//        ad->done = ETrue;
//      }
//      break;
//    }
//    else if (c == '\n') {
//      x = 20;
//      y += LINE_HEIGHT;
//    }
//    else {
//      x += Font::write(x, y, c);
//      i++;
//    }
//  }
//  // if (game_mode == MODE_CREDITS) {
//  Font::scale = 512;
//  // }
//  me->sleep(1);
//}
//
//
//void GAttractModeState::entry(Process *me, Object *o) {
//  attract_data *ad = (attract_data *)&o->x;
//  ad->screen = 0;
//  init_screen(ad);
//  gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_SCREEN_WAV);
//
//  me->sleep(1, typewriter);
//}
