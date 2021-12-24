#include "BA80.h"
#include <Arduino.h>
#include "config.h"
#include "BA80Keyboard.h"


uint8_t ba80_keyboard_init_sequence[] = {
        0xe0,
        0x07,
        0xcd,
        0x07,
        0xc0,
        0x06,
        0xc0,
        0xc5,
        0xc0,
        0x04,
        0xc0,
        0x03,
        0xc0,
        0x02,
        0xc0,
        0x01
    };



BA80::BA80(HardwareSerial* serialInterface)
{
    _serialInterface = serialInterface;
    _txLength = 0;
    _txBufferPointer = 0;
    _lastTransmitTime = 0;
    _lastTxBufferPointer = 0;
}

void BA80::begin()
{
    _serialInterface->begin(BA80_SERIAL_BAUD, BA80_SERIAL_CONF);
    _state = WaitingForVDU;
}

void BA80::handle()
{
    uint8_t response; 
    bool ackReceived = false;

    if(_serialInterface->available())
    {
        response = _serialInterface->read() & 0xff;
        if(response == BA80_CMD_VDU_READY)
        {
            // VDU is ready -> send init sequence
            _state = Initializing;
            transmit(ba80_keyboard_init_sequence, sizeof(ba80_keyboard_init_sequence));
        }
        else if(response == BA80_CMD_VDU_ACK)
        {
            ackReceived = true;
            if(_state == Initializing && transmissionDone())
                _state = Ready;
        }
    }

    if(_state == WaitingForVDU && millis() - _lastTransmitTime >= BA80_KEYBOARD_READY_INTERVAL )
    {
        transmit(BA80_CMD_KEYBOARD_READY);
    }
    
    // Transmit data if buffer is not empty. Only one byte can be transmitted until ACK was received.
    if(_txLength != 0 && _txBufferPointer < _txLength && (_txBufferPointer == 0 || ackReceived))
    {
        _serialInterface->write(_txBuffer[_txBufferPointer]);
        _lastTransmitTime = millis();
        _txBufferPointer++;
        _lastTxBufferPointer = _txBufferPointer;
    }

    if(!transmissionDone() && _lastTxBufferPointer == _txBufferPointer && millis() - _lastTransmitTime >= 300)
    {
        _txLength = 0;
        _txBufferPointer = 0;
        _lastTxBufferPointer = 0;
    }
}

bool BA80::transmissionDone()
{
    return _txBufferPointer == _txLength;
}

bool BA80::readyForKeyboardInput()
{
    return _state == Ready || transmissionDone();
}

void BA80::sendKeyDown(uint8_t keyCode)
{
    if(!readyForKeyboardInput())
        return;

    uint8_t data[] = { 
        BA80_CMD_KEY_PRESSED, 
        keyCode 
    };

    transmit(data, 2);
}

void BA80::sendKeyUp(uint8_t keyCode) 
{
    if(!readyForKeyboardInput())
        return;

    uint8_t data[] = { 
        BA80_CMD_KEY_RELEASED, 
        keyCode 
    };

    transmit(data, 2);    
}

void BA80::transmit(uint8_t data)
{
    _txBuffer[0] = data;
    _txLength = 1;
    _txBufferPointer = 0;
}

void BA80::transmit(uint8_t* data, uint8_t len)
{
    if(len > BA80_TX_BUFFER_SIZE)
        return;

    memcpy(&_txBuffer, data, len);
    _txBufferPointer = 0;
    _txLength = len;
}

void BA80::sendPowerOff()
{
    uint8_t data[] = { 
        0xc0, 
        0x20 
    };

    transmit(data, 2); 
}