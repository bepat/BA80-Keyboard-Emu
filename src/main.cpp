#include <Arduino.h>
#include "config.h"
#include "BA80.h"
#include <PS2KeyAdvanced.h>
#include <Bounce2.h>
#include "BA80Keyboard.h"

PS2KeyAdvanced _ps2Keyboard;
BA80 _ba80(&Serial);
Bounce _powerButton = Bounce();

uint8_t _keyTranslation[] = {
    BA80_KEY_IGNORE,
    BA80_KEY_IGNORE, // PS2_KEY_NUM (0x01) 
    BA80_KEY_IGNORE, // PS2_KEY_SCROLL (0x02) 
    BA80_KEY_CAPSLOCK, // PS2_KEY_CAPS (0x03) 
    BA80_KEY_IGNORE, // PS2_KEY_PRTSCR (0x04) 
    BA80_KEY_IGNORE, // PS2_KEY_PAUSE (0x05) 
    BA80_KEY_SHIFTL, // PS2_KEY_L_SHIFT (0x06) 
    BA80_KEY_SHIFTR, // PS2_KEY_R_SHIFT (0x07) 
    BA80_KEY_CTRL, // PS2_KEY_L_CTRL (0x08) 
    BA80_KEY_TC, // PS2_KEY_R_CTRL (0x09) 
    BA80_KEY_ALTL, // PS2_KEY_L_ALT (0x0A) 
    BA80_KEY_R, // PS2_KEY_R_ALT (0x0B) 
    BA80_KEY_IGNORE, // PS2_KEY_L_GUI (0x0C) 
    BA80_KEY_IGNORE, // PS2_KEY_R_GUI (0x0D) 
    BA80_KEY_IGNORE, // PS2_KEY_MENU (0x0E) 
    BA80_KEY_IGNORE, // PS2_KEY_BREAK (0x0F) 
    BA80_KEY_IGNORE, // PS2_KEY_SYSRQ (0x10) 
    BA80_KEY_X4, // PS2_KEY_HOME (0x11) 
    BA80_KEY_X5, // PS2_KEY_END (0x12) 
    BA80_KEY_YES, // PS2_KEY_PGUP (0x13) 
    BA80_KEY_NO, // PS2_KEY_PGDN (0x14) 
    BA80_KEY_LEFT, // PS2_KEY_L_ARROW (0x15) 
    BA80_KEY_RIGHT, // PS2_KEY_R_ARROW (0x16) 
    BA80_KEY_UP, // PS2_KEY_UP_ARROW (0x17) 
    BA80_KEY_DOWN, // PS2_KEY_DN_ARROW (0x18) 
    BA80_KEY_INS, // PS2_KEY_INSERT (0x19) 
    BA80_KEY_DEL, // PS2_KEY_DELETE (0x1A) 
    BA80_KEY_ESC, // PS2_KEY_ESC (0x1B) 
    BA80_KEY_BS, // PS2_KEY_BS (0x1C) 
    BA80_KEY_TAB, // PS2_KEY_TAB (0x1D) 
    BA80_KEY_RETURN, // PS2_KEY_ENTER (0x1E) 
    BA80_KEY_SPACE, // PS2_KEY_SPACE (0x1F) 
    BA80_KEY_NUM_0, // PS2_KEY_KP0 (0x20) 
    BA80_KEY_NUM_1, // PS2_KEY_KP1 (0x21) 
    BA80_KEY_NUM_2, // PS2_KEY_KP2 (0x22) 
    BA80_KEY_NUM_3, // PS2_KEY_KP3 (0x23) 
    BA80_KEY_NUM_4, // PS2_KEY_KP4 (0x24) 
    BA80_KEY_NUM_5, // PS2_KEY_KP5 (0x25) 
    BA80_KEY_NUM_6, // PS2_KEY_KP6 (0x26) 
    BA80_KEY_NUM_7, // PS2_KEY_KP7 (0x27) 
    BA80_KEY_NUM_8, // PS2_KEY_KP8 (0x28) 
    BA80_KEY_NUM_9, // PS2_KEY_KP9 (0x29) 
    BA80_KEY_NUM_COMMA, // PS2_KEY_KP_DOT (0x2A) 
    BA80_KEY_NUM_RETURN, // PS2_KEY_KP_ENTER (0x2B) 
    BA80_KEY_NUM_PLUS, // PS2_KEY_KP_PLUS (0x2C) 
    BA80_KEY_NUM_MINUS, // PS2_KEY_KP_MINUS (0x2D) 
    BA80_KEY_NUM_MULTIPLICATE, // PS2_KEY_KP_TIMES (0x2E) 
    BA80_KEY_NUM_DIVIDE, // PS2_KEY_KP_DIV (0x2F) 
    BA80_KEY_0, // PS2_KEY_0 (0x30) 
    BA80_KEY_1, // PS2_KEY_1 (0x31) 
    BA80_KEY_2, // PS2_KEY_2 (0x32) 
    BA80_KEY_3, // PS2_KEY_3 (0x33) 
    BA80_KEY_4, // PS2_KEY_4 (0x34) 
    BA80_KEY_5, // PS2_KEY_5 (0x35) 
    BA80_KEY_6, // PS2_KEY_6 (0x36) 
    BA80_KEY_7, // PS2_KEY_7 (0x37) 
    BA80_KEY_8, // PS2_KEY_8 (0x38) 
    BA80_KEY_9, // PS2_KEY_9 (0x39) 
    BA80_KEY_AE, // PS2_KEY_APOS (0x3A) 
    BA80_KEY_COMMA, // PS2_KEY_COMMA (0x3B) 
    BA80_KEY_SHARPS, // PS2_KEY_MINUS (0x3C) 
    BA80_KEY_DOT, // PS2_KEY_DOT (0x3D) 
    BA80_KEY_MINUS, // PS2_KEY_DIV (0x3E) 
    BA80_KEY_IGNORE, // PS2_KEY_KP_EQUAL (0x3F) 
    BA80_KEY_LEFTUP, // PS2_KEY_SINGLE (0x40) 
    BA80_KEY_A, // PS2_KEY_A (0x41) 
    BA80_KEY_B, // PS2_KEY_B (0x42) 
    BA80_KEY_C, // PS2_KEY_C (0x43) 
    BA80_KEY_D, // PS2_KEY_D (0x44) 
    BA80_KEY_E, // PS2_KEY_E (0x45) 
    BA80_KEY_F, // PS2_KEY_F (0x46) 
    BA80_KEY_G, // PS2_KEY_G (0x47) 
    BA80_KEY_H, // PS2_KEY_H (0x48) 
    BA80_KEY_I, // PS2_KEY_I (0x49) 
    BA80_KEY_J, // PS2_KEY_J (0x4A) 
    BA80_KEY_K, // PS2_KEY_K (0x4B) 
    BA80_KEY_L, // PS2_KEY_L (0x4C) 
    BA80_KEY_M, // PS2_KEY_M (0x4D) 
    BA80_KEY_N, // PS2_KEY_N (0x4E) 
    BA80_KEY_O, // PS2_KEY_O (0x4F) 
    BA80_KEY_P, // PS2_KEY_P (0x50) 
    BA80_KEY_Q, // PS2_KEY_Q (0x51) 
    BA80_KEY_R, // PS2_KEY_R (0x52) 
    BA80_KEY_S, // PS2_KEY_S (0x53) 
    BA80_KEY_T, // PS2_KEY_T (0x54) 
    BA80_KEY_U, // PS2_KEY_U (0x55) 
    BA80_KEY_V, // PS2_KEY_V (0x56) 
    BA80_KEY_W, // PS2_KEY_W (0x57) 
    BA80_KEY_X, // PS2_KEY_X (0x58) 
    BA80_KEY_Z, // PS2_KEY_Y (0x59) 
    BA80_KEY_Y, // PS2_KEY_Z (0x5A) 
    BA80_KEY_OE, // PS2_KEY_SEMI (0x5B) 
    BA80_KEY_ANGLEBRACKET, // PS2_KEY_BACK (0x5C) 
    BA80_KEY_UE, // PS2_KEY_OPEN_SQ (0x5D) 
    BA80_KEY_PLUS, // PS2_KEY_CLOSE_SQ (0x5E) 
    BA80_KEY_ACCENT, // PS2_KEY_EQUAL (0x5F) 
    BA80_KEY_IGNORE, // PS2_KEY_KP_COMMA (0x60) 
    BA80_KEY_F1, // PS2_KEY_F1 (0x61) 
    BA80_KEY_F2, // PS2_KEY_F2 (0x62) 
    BA80_KEY_F3, // PS2_KEY_F3 (0x63) 
    BA80_KEY_F4, // PS2_KEY_F4 (0x64) 
    BA80_KEY_F5, // PS2_KEY_F5 (0x65) 
    BA80_KEY_F6, // PS2_KEY_F6 (0x66) 
    BA80_KEY_F7, // PS2_KEY_F7 (0x67) 
    BA80_KEY_F8, // PS2_KEY_F8 (0x68) 
    BA80_KEY_F9, // PS2_KEY_F9 (0x69) 
    BA80_KEY_END, // PS2_KEY_F10 (0x6A) 
    BA80_KEY_CWIN, // PS2_KEY_F11 (0x6B) 
    BA80_KEY_LOC, // PS2_KEY_F12 (0x6C) 
    BA80_KEY_IGNORE, // PS2_KEY_F13 (0x6D) 
    BA80_KEY_IGNORE, // PS2_KEY_F14 (0x6E) 
    BA80_KEY_IGNORE, // PS2_KEY_F15 (0x6F) 
    BA80_KEY_IGNORE, // PS2_KEY_F16 (0x70) 
    BA80_KEY_IGNORE, // PS2_KEY_F17 (0x71) 
    BA80_KEY_IGNORE, // PS2_KEY_F18 (0x72) 
    BA80_KEY_IGNORE, // PS2_KEY_F19 (0x73) 
    BA80_KEY_IGNORE, // PS2_KEY_F20 (0x74) 
    BA80_KEY_IGNORE, // PS2_KEY_F21 (0x75) 
    BA80_KEY_IGNORE, // PS2_KEY_F22 (0x76) 
    BA80_KEY_IGNORE, // PS2_KEY_F23 (0x77) 
    BA80_KEY_IGNORE, // PS2_KEY_F24 (0x78) 
    BA80_KEY_IGNORE, // PS2_KEY_NEXT_TR (0x79) 
    BA80_KEY_IGNORE, // PS2_KEY_PREV_TR (0x7A) 
    BA80_KEY_IGNORE, // PS2_KEY_STOP (0x7B) 
    BA80_KEY_IGNORE, // PS2_KEY_PLAY (0x7C) 
    BA80_KEY_IGNORE, // PS2_KEY_MUTE (0x7D) 
    BA80_KEY_IGNORE, // PS2_KEY_VOL_UP (0x7E) 
    BA80_KEY_IGNORE, // PS2_KEY_VOL_DN (0x7F) 
    BA80_KEY_IGNORE, // PS2_KEY_MEDIA (0x80) 
    BA80_KEY_IGNORE, // PS2_KEY_EMAIL (0x81) 
    BA80_KEY_IGNORE, // PS2_KEY_CALC (0x82) 
    BA80_KEY_IGNORE, // PS2_KEY_COMPUTER (0x83) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_SEARCH (0x84) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_HOME (0x85) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_BACK (0x86) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_FORWARD (0x87) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_STOP (0x88) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_REFRESH (0x89) 
    BA80_KEY_IGNORE, // PS2_KEY_WEB_FAVOR (0x8A) 
    BA80_KEY_IGNORE, // PS2_KEY_EUROPE2 (0x8B) 
    BA80_KEY_IGNORE, // PS2_KEY_POWER (0x8C) 
    BA80_KEY_IGNORE, // PS2_KEY_SLEEP (0x8D) 
    BA80_KEY_IGNORE, // PS2_KEY_WAKE (0x90) 
    BA80_KEY_IGNORE, // PS2_KEY_INTL1 (0x91) 
    BA80_KEY_IGNORE, // PS2_KEY_INTL2 (0x92) 
    BA80_KEY_IGNORE, // PS2_KEY_INTL3 (0x93) 
    BA80_KEY_IGNORE, // PS2_KEY_INTL4 (0x94) 
    BA80_KEY_IGNORE, // PS2_KEY_INTL5 (0x95) 
    BA80_KEY_IGNORE, // PS2_KEY_LANG1 (0x96) 
    BA80_KEY_IGNORE, // PS2_KEY_LANG2 (0x97) 
    BA80_KEY_IGNORE, // PS2_KEY_LANG3 (0x98) 
    BA80_KEY_IGNORE, // PS2_KEY_LANG4 (0x99) 
    BA80_KEY_IGNORE, // PS2_KEY_LANG5 (0xA0) 
};


void setup() 
{
    _ps2Keyboard.begin(PS2_DATA_PIN, PS2_CLK_PIN);
    _ps2Keyboard.setNoRepeat(1);
    _ps2Keyboard.setLock(PS2_LOCK_NUM | PS2_LOCK_CAPS);
    _ba80.begin();
    _powerButton.attach(POWER_BUTTON_PIN, INPUT_PULLUP);
    _powerButton.interval(50);
}

void loop() 
{
    uint16_t keyData;

    _ba80.handle();
    _powerButton.update();

    if(_ba80.readyForKeyboardInput())
    {
        if(_powerButton.fell())
            _ba80.sendPowerOff();
        else if(_ps2Keyboard.available())
        {
            keyData = _ps2Keyboard.read();

            uint8_t trans = BA80_KEY_IGNORE;

            if((keyData & 0xff) < 0xa1)
                trans = _keyTranslation[keyData & 0xff];

            if(trans != BA80_KEY_IGNORE) {
                if(keyData & PS2_BREAK) 
                    _ba80.sendKeyUp(trans & 0xff);
                else
                    _ba80.sendKeyDown(trans & 0xff);   
            }
        }
    }
}