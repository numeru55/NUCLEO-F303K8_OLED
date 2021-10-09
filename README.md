Thanks to:

https://www.microchip.com/forums/m826100-p2.aspx

https://www.microchip.com/forums/download.axd?file=0;831415

https://neo-sahara.com/wp/2017/11/16/pic24fj64gb002-ssd1306gm009605/

# OLED

- SSD1306 module, I2C, 128x64 or 128x32.
- I2C address is 0x78. 0x3c is for Arduino.

# Demo Run

- Connect OLED to the right side of NUCLEO as follows.

```
Arduino STM32             OLED MODULE
 Label   pin
 [VIN]
 [GND] ------------------ GND
 [RST]
 [5V]
 [A7]   [PA2]
 [A6]   [PA7]
 [A5]   [PA6 | PB6] ----- SCL
 [A4]   [PA5 | PB7] ----- SDA
 [A3]   [PA4]
 [A2]   [PA3]
 [A1]   [PA1]
 [A0]   [PA0]
 [REF]
 [3V3] ------------------ VCC
 [D13]  [PB3]
```

Please note that my Nucleo soldered SB16 and SB18, so [A5] connects PA6 and PB6, [A4] to PA5 and PB7.

- Choose OLED size in SSD1306_I2C.h define:

```cpp:
#define OLED_HEIGHT 32
// #define OLED_HEIGHT 64
```

- Build and run. Tested only for 128x32, with STM32CubeIDE 1.7.0.

# Notice
- This is not required "Display Buffer." Sample program wrote data to OLED, and forget it.
- I hope this mechanism is suitable for small MCU. Original my source was run at PIC16F chip.