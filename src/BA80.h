/*
  The MIT License (MIT)

  Copyright (c) 2022 Benedikt Patt

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _BA80_H_
#define _BA80_H_

#include <Arduino.h>

//settings for serial connection to VDU
#define BA80_SERIAL_BAUD                4800
#define BA80_SERIAL_CONF                SERIAL_8O1
#define BA80_TX_BUFFER_SIZE             32
#define BA80_KEYBOARD_READY_INTERVAL    50

//BA80 commands
#define BA80_CMD_KEY_PRESSED    0xa0
#define BA80_CMD_KEY_RELEASED   0xa8
#define BA80_CMD_KEYBOARD_READY 0xf0
#define BA80_CMD_VDU_READY      0x10
#define BA80_CMD_VDU_ACK        0x80

enum BA80StateEnum {
    WaitingForVDU = 0,
    Initializing = 1,
    Ready = 3,
    SendingKey = 4
};

class BA80 {
    protected:
        HardwareSerial* _serialInterface;
        BA80StateEnum _state;
        uint8_t _txBuffer[BA80_TX_BUFFER_SIZE];
        uint8_t _txBufferPointer;
        uint8_t _lastTxBufferPointer;
        uint8_t _txLength;
        unsigned long _lastTransmitTime;

        void transmit(uint8_t* data, uint8_t len);
        void transmit(uint8_t data);
        bool transmissionDone();

    public:
        BA80(HardwareSerial* serialInterface);
        void begin();
        void handle();
        bool readyForKeyboardInput();
        void sendKeyDown(uint8_t keyCode);
        void sendKeyUp(uint8_t keyCode);
        void sendPowerOff();
};

#endif