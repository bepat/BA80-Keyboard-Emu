#ifndef _CONFIG_H_
#define _CONFIG_H_

//settings for PS2 keyboard
#define PS2_CLK_PIN     2 // PD2 (has to be an external interrupt pin)
#define PS2_DATA_PIN    3 // PD3

#define POWER_BUTTON_PIN    A3 // PC3

#if defined( ARDUINO_ARCH_AVR )
#define BA80_REQUIRES_PROGMEM    1
#endif



#endif