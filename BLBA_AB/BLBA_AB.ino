/*
  Blob Attack: https://t-arg.github.io/ID-28-Blob-Attack/

  Arduboy version 2.0.0:  https://github.com/T-arg/ID-28-Blob-Attack

  MADE by TEAM a.r.g. : https://t-arg.github.io/

  2015 - Game by JO3RI - All art by CastPixel: https://twitter.com/castpixel

  2026 - MUSIC and SOUND EFFECTS - JO3RI

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 28

#include "globals.h"
#include "menu.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateMenuJbox,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};


void setup() {
  arduboy.boot();                                           
  arduboy.audio.begin();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  musicUpdate();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

