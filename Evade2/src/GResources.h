#ifndef MODITE_GRESOURCES_H
#define MODITE_GRESOURCES_H

#include <BTypes.h>
#include "Resources.h"

// Events
const TUint16 EVENT_SPELL_PROCESS_EXIT = 1;

enum {
  FONT_8x8_SLOT,
  FONT_16x16_SLOT,
  SLOT_MAX,
};

// BRaw slots
enum {
  SONG0_SLOT,
  SONG1_SLOT,
  SONG2_SLOT,
  SONG3_SLOT,
  SONG4_SLOT,
  SONG5_SLOT,
  SONG6_SLOT,
  SONG7_SLOT,
  SONG8_SLOT,
  SONG9_SLOT,
  SONG10_SLOT,
  SONG11_SLOT,
  SFX1_SLOT,
  SFX2_SLOT,
  SFX3_SLOT,
  SFX4_SLOT,
  SFX5_SLOT,
  SFX6_SLOT,
  SFX7_SLOT,
  SFX8_SLOT,
  SFX9_SLOT,
  SFX10_SLOT,
  SFX11_SLOT,
  SFX12_SLOT,
  SFX13_SLOT,
  SFX15_SLOT,
  SFX16_SLOT,
  SFX17_SLOT,
  SFX18_SLOT,
  SFX19_SLOT,
  SFX20_SLOT,
  SFX21_SLOT,
  SFX22_SLOT,
  SFX23_SLOT,
  SFX24_SLOT,
  SFX25_SLOT,
  SFX26_SLOT,
  SFX27_SLOT,
  SFX28_SLOT,
  SFX29_SLOT,
  SFX30_SLOT,
  SFX31_SLOT,
  SFX32_SLOT,
  SFX33_SLOT,
  SFX34_SLOT,
  SFX35_SLOT,
  SFX36_SLOT,
  SFX37_SLOT,
};

#if 0
// SPLASH_SPRITES.BMP
const TUint16 IMG_DROP1 = 0;
const TUint16 IMG_DROP2 = 4;
const TUint16 IMG_DROP3 = 8;
const TUint16 IMG_DROP4 = 16;
const TUint16 IMG_DROP5 = 24;
const TUint16 IMG_DROP6 = 32;

// PLAYER.BMP
const TUint16 IMG_IDLE = 0;
const TUint16 IMG_SELECTED = 4;
const TUint16 IMG_WALK_DOWN = 8;
const TUint16 IMG_WALK_RIGHT = 12;
const TUint16 IMG_WALK_UP = 16;
const TUint16 IMG_SWORD_DOWN = 20;
const TUint16 IMG_SWORD_RIGHT = 24;
const TUint16 IMG_SWORD_UP = 28;
const TUint16 IMG_DAMAGE_DOWN = 32;
const TUint16 IMG_DAMAGE_RIGHT = 36;
const TUint16 IMG_DAMAGE_UP = 40;
#endif


//// RESERVED COLORS

const TUint8 COLOR_BLACK = 0;
const TUint8 COLOR_WHITE = 1;
const TUint8 COLOR_TEXT = 2;
const TUint8 COLOR_STAR = 3;
const TUint8 COLOR_ENEMY1 = 4;
const TUint8 COLOR_SHMOO = 255;

const TUint8 COLOR_BLUE = 201;
const TUint8 COLOR_GREEN = 202;
const TUint8 COLOR_CYAN = 203;
const TUint8 COLOR_RED = 204;
const TUint8 COLOR_MAGENTA = 205;
const TUint8 COLOR_BROWN = 206;
const TUint8 COLOR_GRAY = 207;
const TUint8 COLOR_DARK_GRAY = 208;
const TUint8 COLOR_BRIGHT_BLUE = 209;
const TUint8 COLOR_BRIGHT_GREEN = 210;
const TUint8 COLOR_BRIGHT_CYAN = 211;
const TUint8 COLOR_BRIGHT_RED = 212;
const TUint8 COLOR_BRIGHT_MAGENTA = 213;
const TUint8 COLOR_YELLOW = 214;
const TUint8 COLOR_SPACE = 215;






const TUint8 SHMOO_COLOR = 216;
const TUint8 BULLET_COLOR = 217;
const TUint8 EBULLET_COLOR = 218;
const TUint8 BOMB_COLOR = 219;
const TUint8 BOSS_COLOR = 220;
const TUint8 ASSAULT_COLOR = 221;
const TUint8 BOMBER_COLOR = 222;
const TUint8 SCOUT_COLOR = 223;
const TUint8 STAR_COLOR = 224;

#define MAX_BBITMAP 5

#endif //MODITE_GRESOURCES_H
