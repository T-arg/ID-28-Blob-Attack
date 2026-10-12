#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include "ATMconfig.h"
#include <ATMlib.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO              0
#define STATE_MENU_MAIN               1
#define STATE_MENU_HELP               2
#define STATE_MENU_PLAY               3
#define STATE_MENU_INFO               4
#define STATE_MENU_SOUNDFX            5
#define STATE_MENU_JBOX               6

//define game states (on main menu)
#define STATE_GAME_PLAYING            7
#define STATE_GAME_PAUSE              8
#define STATE_GAME_OVER               9

#define SPEED_STATE_START             60
#define SPEED_STATE_ONE               55
#define SPEED_STATE_TWO               50
#define SPEED_STATE_THREE             45
#define SPEED_STATE_FOUR              40
#define SPEED_STATE_FIVE              30
#define SPEED_STATE_SIX               20
#define SPEED_STATE_SEVEN             10


Arduboy2Base arduboy;
Sprites sprites;

// draws a run-length compressed bitmap, 64 pixels high, at y = 0 (pixels are OR-ed into the screen buffer, like drawSelfMasked)
// stream: a byte with bit 7 set = a run (low 7 bits = length) of the byte that follows; otherwise the byte is the number of literal bytes that follow
void drawRle(const uint8_t *p, byte x0, byte w)
{
  uint8_t *b = arduboy.getBuffer() + x0;
  byte col = 0;
  for (int left = w * 8; left > 0;)
  {
    byte h = pgm_read_byte(p++);
    byte n = h & 0x7F;
    byte v = (h & 0x80) ? pgm_read_byte(p++) : 0;
    left -= n;
    while (n--)
    {
      if (!(h & 0x80)) v = pgm_read_byte(p++);
      *b++ |= v;
      if (++col == w) { col = 0; b += 128 - w; }
    }
  }
}
ATMsynth ATM;

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
unsigned long scorePlayer;
byte currentSpeed;
byte previousSpeed;


#include "music.h"

#endif
