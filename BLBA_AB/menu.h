#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

byte sequenceFrame;
const unsigned char PROGMEM frameSequences[] = {
  // bouncing Ball Frame Sequence
  0, 1, 2, 0, 0, 0, 0, 0,
  // bouncing Ball Height Sequence
  0, 0, 0, 1, 0, 0, 0, 0,
  // game title Height Sequence
  1, 2, 3, 4, 5, 4, 3, 2,
};

void drawTitleScreen()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  drawRle(splashScreen, 0, 128);
}

void stateMenuIntro()
{
  globalCounter++;
  if (globalCounter == 20) sfx(sfxLetters, 1);                   // the letter notes, one every 17 frames
  if (globalCounter < 130)                                       // the letters of ARDUBOY slide in from the right, one after the other
  {
    for (byte i = 0; i < 7; i++)
    {
      int d = globalCounter - 17 * i;                            // frames since this letter started to move
      if (d < 0) break;
      int x = 36 + 8 * i;
      if (d < 20) x += 6 * (20 - d);
      sprites.drawSelfMasked(x, 28, arduboyLetters, i);
    }
  }
  else sprites.drawSelfMasked(49, 20, T_arg, 0);
  if (globalCounter > 254)
  {
    gameState = STATE_MENU_MAIN;
    globalCounter = 0;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  sprites.drawPlusMask(22 + (22 * (menuSelection - 3)), 52, selectorOne_plus_mask, 0);
  sprites.drawErase(24, 54, mainMenu, 0);
  sprites.drawPlusMask(30 + (22 * (menuSelection - 3)), 44 - pgm_read_byte(&frameSequences[sequenceFrame + 8]), bouncingBall_plus_mask, pgm_read_byte(&frameSequences[sequenceFrame]));
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 6)) { menuSelection++; sfx(sfxMove, 1); }
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 3)) { menuSelection--; sfx(sfxMove, 1); }
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) { gameState = menuSelection; sfx(sfxSelect, 1); }
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(19, 27, madeBy, 0);
  sprites.drawSelfMasked(5, pgm_read_byte(&frameSequences[sequenceFrame + 16]), gameTitle, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  sprites.drawPlusMask(61 + (19 * arduboy.audio.enabled()), 52, selectorTwo_plus_mask, 0);
  sprites.drawErase(31, 54, soundMenu, 0);
  sprites.drawPlusMask(67 + (19 * arduboy.audio.enabled()), 44 - pgm_read_byte(&frameSequences[sequenceFrame + 8]), bouncingBall_plus_mask, pgm_read_byte(&frameSequences[sequenceFrame]));
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
    sfx(sfxSelect, 1);
  }
}

// the jukebox: MUSIC (UP) and SNDFX (DOWN), LEFT = next number, RIGHT = previous number, B = play, A = stop (and leave when nothing plays)
byte jbRow = 0;
byte jbNr[2];                                  // MUSIC 0..2, SNDFX 0..7

// little arrows left and right of the number (2 x 3 pixels, frame 0 points left, frame 1 right)
const unsigned char PROGMEM jbArrow[] = { 2, 8, 0x02, 0x07, 0x07, 0x02 };

void stateMenuJbox()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(5, pgm_read_byte(&frameSequences[sequenceFrame + 16]), gameTitle, 0);
  for (byte i = 0; i < 2; i++)
  {
    const unsigned char *w = i ? jboxSndfx : jboxMusic;
    byte y = 34 + 12 * i;
    sprites.drawSelfMasked(73, y + 2, jbArrow, 0);
    sprites.drawSelfMasked(86, y + 2, jbArrow, 1);
    arduboy.fillRect(44, y, pgm_read_byte(w), 7, WHITE);
    sprites.drawErase(44, y, w, 0);
    if (jbRow != i || (arduboy.frameCount & 16))              // the selected number blinks
    {
      arduboy.fillRect(76, y, 9, 7, WHITE);
      sprites.drawErase(76, y, jboxNumbers, (jbNr[i] + 1) / 10);
      sprites.drawErase(80, y, jboxNumbers, (jbNr[i] + 1) % 10);
    }
  }
  byte n = jbRow ? JBOX_SFX : JBOX_MUSIC;
  if (arduboy.justPressed(UP_BUTTON)) jbRow = 0;
  if (arduboy.justPressed(DOWN_BUTTON)) jbRow = 1;
  if (arduboy.justPressed(RIGHT_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + 1) % n;
  if (arduboy.justPressed(LEFT_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + n - 1) % n;
  if (arduboy.justPressed(B_BUTTON) && arduboy.audio.enabled())
  {
    if (jbRow) sfx((const uint8_t *)pgm_read_word(&sfxList[jbNr[1]]), jbNr[1] ? 1 : 3);
    else musicPlay(jbNr[0]);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    if (musicId == MUSIC_OFF) gameState = STATE_MENU_MAIN;    // nothing playing: leave, the title tune restarts
    else musicPlay(MUSIC_OFF);
  }
}

#endif
