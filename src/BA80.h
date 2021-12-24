#ifndef _BA80_H_
#define _BA80_H_

#include <Arduino.h>

//settings for serial connection to VDU
#define BA80_SERIAL_BAUD    4800
#define BA80_SERIAL_CONF    SERIAL_8O1
#define BA80_TX_BUFFER_SIZE 32
#define BA80_KEYBOARD_READY_INTERVAL    50


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