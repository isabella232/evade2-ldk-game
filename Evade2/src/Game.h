#ifndef MODITE_GAME_H
#define MODITE_GAME_H

#include <BTypes.h>

#define WIDTH 640
#define HEIGHT 400
//#define SCREEN_WIDTH 320
//#define SCREEN_HEIGHT 200
//#define SCREEN_DEPTH 8

const TFloat CAMERA_VZ = 2; // 4;
const TFloat DELTACONTROL = 6; // 11;
const TInt8 MAX_BULLETS = 6;
const TFloat BULLET_VZ = 8; // 15;

// COLLISION_RADIUS = distance from player bullet to enemy required for a hit
const TFloat COLLISION_RADIUS = 64;
#define BANK_LEFT TUint32(1<<OFLAG_USER_BIT)
#define ORBIT_LEFT TUint32(1<<(OFLAG_USER_BIT+1))

#define FRAME_RATE_INFO 0
#undef FRAME_RATE_INFO

#define ENABLE_AUDIO
#undef ENABLE_AUDIO

#define ENABLE_ROTATING_TEXT
//#undef ENABLE_ROTATING_TEXT

#define ENABLE_OPTIONS
// #undef ENABLE_OPTIONS

#define ENABLE_HUD_MOVEMENTS
// #undef ENABLE_HUD_MOVEMENTS

// Debug mode
#define DEBUG_MODE
// #undef DEBUG_MODE

// special characters
#define STR_LEFT_ARROW "\xf"
#define STR_RIGHT_ARROW "\xe"

#ifndef PI
#define PI 3.1415926
#endif

#ifndef RADIANS
#define RADIANS(degrees) (TFloat(degrees) * PI / 180)
#endif

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>
#include <Controls.h>

//const TUint16 FACTOR = FRAMES_PER_SECOND / 30;

#include "GResources.h"
#include "common/GVectorFont.h"
#include "GGame.h"
//#include "common/GDialogWidget.h"
//#include "common/GButtonWidget.h"
#include "TOptions.h"
#include "GSoundPlayer.h"

// GGameEngine states
//#include "GSplashState.h"
//#include "GGameState.h"
//#include "GMainMenuState.h"
//#include "GMainOptionsState.h"
//#include "GResetOptionsState.h"
//#include "GGameMenuState.h"
//#include "GAttractModeState.h"
//#include "GVictoryState.h"
//#include "DebugMenuState/GDebugMenuState.h"
//#include "GCreditsState.h"
//#include "GInventory.h"

extern BViewPort   *gViewPort;
extern TOptions    *gOptions;


/**
 * Control bits definitions
 */
const TUint16 CONTROL_JOYUP = JOYUP;
const TUint16 CONTROL_JOYDOWN = JOYDOWN;
const TUint16 CONTROL_JOYLEFT = JOYLEFT;
const TUint16 CONTROL_JOYRIGHT = JOYRIGHT;
const TUint16 CONTROL_FIRE = BUTTONA;
const TUint16 CONTROL_BURST = BUTTONB;
/* const TUint16 CONTROL_SPELL = BUTTONX; */
/* const TUint16 CONTROL_INVENTORY = BUTTONR; */
/* const TUint16 CONTROL_TARGET = BUTTONL; */
const TUint16 CONTROL_DEBUG = BUTTON_SELECT;

struct EnemyConfig {
  EnemyConfig() {
    mX = mY = mZ = 0;
    mVX = mVY = mVZ = 0;
  }
  TInt16 mX;
  TInt16 mY;
  TInt16 mZ;
  TInt16 mVX;
  TInt16 mVY;
  TInt16 mVZ;
};

//const TUint16 OFLAG_TYPE_MASK = 0x07;
//// Object is an enemy
//const TUint16 OTYPE_ENEMY = 0;
//// Object is player's bullet
//const TUint16 OTYPE_PLAYER_BULLET = 1;
//// Object is enemy bullet
//const TUint16 OTYPE_ENEMY_BULLET = 2;
//// STRING means lines is a character string to be rendered in 3D
//const TUint16 OTYPE_STRING = 3;
//const TUint16 OTYPE_ASTEROID = 4;
//// MOON means lines is ignored and a circle is rendered, as in a moon or planet
//// theta becomes radius
//const TUint16 OTYPE_MOON = 5;

// FLAGS
// if set, the lines will explode
const TUint16 OFLAG_EXPLODE = (1 << 4);
// set when the object has collided (ENEMY vs PLAYER BULLET, etc.)
const TUint16 OFLAG_COLLISION = (1 << 5);
// Process can use the remaining bits, starting with (1<<USER_BIT)
const TUint16 OFLAG_USER_BIT = (6);

#endif //MODITE_GAME_H
