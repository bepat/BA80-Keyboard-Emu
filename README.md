# BA80 Keyboard Emulation
Keyboards for the Nixdorf BA80 VDU (Visual Display Unit) are getting rare due tu people who scrap them for getting Cherry MX keys and key caps.
This little project uses an Atmel AVR with Arduino framework (PlatformIO) to adapt a simple PS2 keyboard for using it as BA80 keyboard.

I figured out the interfacing and protocol details, used to connect the keyboard the the BA80.
Details on this can be found [here](doc/BA80-keyboard.md).

# Hardware
I used an Atmega8 as MCU, but you can use every other AVR which has a hardware USART (for the VDU side) and a hardware interrupt pin (for the PS2 side).
The assignment is done in the `config.h` file.