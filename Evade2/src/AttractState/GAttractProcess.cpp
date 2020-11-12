//
// Created by Michael Schwartz on 11/4/20.
//

#include "GAttractProcess.h"
#include "GEnemyProcess.h"


static const TInt8 TYPEWRITER_SPEED = 10;
static const TInt8 LINE_HEIGHT      = 26;

static const char scout_text[]   = "SCOUT";
static const char bomber_text[]  = "BOMBER";
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

const TInt8 MAX_SCREEN  = 2;
const TInt8 MAX_CREDITS = 5;

enum {
  STATE_TYPEWRITER,
  STATE_NEXT,
};

GAttractProcess::GAttractProcess() : BProcess() {
//  mSprite = new GVectorSprite();
//  gGameEngine->AddSprite(mSprite);
  auto *ad = (TAttractData *) &mData;
  ad->screen = 0;
  InitScreen();
  gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_SCREEN_WAV, 3);
  mState = STATE_TYPEWRITER;
}

GAttractProcess::~GAttractProcess() {
  //
}

TBool GAttractProcess::RunBefore() {
  return ETrue;
}

TBool GAttractProcess::RunAfter() {
  switch (mState) {
    case STATE_TYPEWRITER:
      return TypewriterState();
    case STATE_NEXT:
      return NextState();
    default:
      Panic("invalid state: mState(%d)", mState);
  }

  return ETrue;
}

void GAttractProcess::InitScreen(TInt16 x, TInt16 y) {
  auto *ad = &mData;
  gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_SCREEN_WAV, 3);

  if (gGame->GetState() == GAME_STATE_ATTRACT_MODE) {
    switch (ad->screen) {
      case 0:
        ad->enemy = ENEMY_SCOUT;
        ad->text  = scout_text;
        x = 125;
        y = 60;
        break;
      case 1:
        ad->enemy = ENEMY_BOMBER;
        ad->text  = bomber_text;
        x = 117;
        y = 60;
        break;
      case 2:
        ad->enemy = ENEMY_ASSAULT;
        ad->text  = assault_text;
        x = 110;
        y = 60;
        break;
    }
  } else {
    ad->enemy = -1;
    switch (ad->screen) {
      case 0:
        ad->text = credits1;
        break;
      case 1:
        ad->text = credits2;
        break;
      case 2:
        ad->text = credits3;
        break;
      case 3:
        ad->text = credits4;
        break;
      case 4:
        ad->text = credits5;
        break;
      case 5:
        ad->text = credits6;
        break;
    }
  }

  ad->offset = 1;
  ad->x      = x;
  ad->y      = y;
  ad->timer  = TYPEWRITER_SPEED;
  ad->done   = EFalse;
}

TBool GAttractProcess::NextState() {
  auto *ad       = (TAttractData *) &mData;
  TInt game_mode = gGame->GetState();

  ad->timer--;
  if (ad->timer < 0) {
    ad->screen++;
    if ((game_mode == GAME_STATE_ATTRACT_MODE && ad->screen > MAX_SCREEN)) {
      gGame->SetState(GAME_STATE_CREDITS);
      return EFalse;
    }
    if (game_mode == GAME_STATE_CREDITS && ad->screen > MAX_CREDITS) {
      gGame->SetState(GAME_STATE_MAIN_MENU);
      return EFalse;
    }
//      Sound::play_sound(SFX_NEXT_ATTRACT_SCREEN);
    InitScreen();
    mState = STATE_TYPEWRITER;
  }
  return ETrue;
}

TBool GAttractProcess::TypewriterState() {
  auto *ad       = (TAttractData *) &mData;
  TInt game_mode = gGame->GetState();

  ad->timer--;

  if (gControls.WasPressed(CONTROL_FIRE)) {
    gGame->SetState(GAME_STATE_GAME);
    return EFalse;
  }

  if (gControls.WasPressed(BUTTON_START)) {
    ad->timer = -1;
    gGame->SetState(GAME_STATE_GAME);
    return EFalse;
  }

  if (ad->timer < 0) {
    if (ad->done) {
      ad->timer = 50;
//      me->sleep(1, next);
      mState = STATE_NEXT;
      return ETrue;
    }
    ad->timer = TYPEWRITER_SPEED;
    ad->offset++;
    gSoundPlayer.TriggerSfx(SFX_NEXT_ATTRACT_CHAR_WAV, 4);
  }

  switch (ad->enemy) {
    case ENEMY_ASSAULT:
      GVectorSprite::DrawVectorGraphic(GEnemyProcess::Graphic(ad->enemy),
                                       TFloat(SCREEN_WIDTH) / 2, TFloat(SCREEN_HEIGHT) / 2, 0.0,
                                       .75, // Originally 2.0
                                       ASSAULT_COLOR);
      break;
    case ENEMY_BOMBER:
      GVectorSprite::DrawVectorGraphic(GEnemyProcess::Graphic(ad->enemy),
                                       TFloat(SCREEN_WIDTH) / 2, TFloat(SCREEN_HEIGHT) / 2, 0.0,
                                       .75, // Originally 2.0
                                       BOMBER_COLOR);
      break;
    case ENEMY_SCOUT:
      GVectorSprite::DrawVectorGraphic(GEnemyProcess::Graphic(ad->enemy),
                                       TFloat(SCREEN_WIDTH) / 2, TFloat(SCREEN_HEIGHT) / 2, 0.0,
                                       .75, // Originally 2.0
                                       SCOUT_COLOR);
      break;
  }
  if (game_mode == GAME_STATE_CREDITS) {
    gVectorFont->scale = 2.0;
  }

  const char *p = ad->text;
  TInt16     x  = ad->x,
             y  = ad->y;

  for (TInt8 i = 0; i < ad->offset;) {
    char c = *p++;
    if (c == '\0') {
      if (!ad->done) {
        ad->timer = 60; // 2 seconds
        ad->done  = ETrue;
      }
      break;
    } else if (c == '\n') {
      x = 20;
      y += LINE_HEIGHT;
    } else {
      x += gVectorFont->write(x, y, c);
      i++;
    }
  }
  // if (game_mode == MODE_CREDITS) {
  gVectorFont->scale = 2;
  // }
  return ETrue;
}

//void Attract::entry() {
//  TAttractData *ad = (TAttractData *) &mData;
//  ad->screen = 0;
//  InitScreen();
//  Sound::play_sound(SFX_NEXT_ATTRACT_SCREEN);
//
//  me->sleep(1, typewriter);
//}
