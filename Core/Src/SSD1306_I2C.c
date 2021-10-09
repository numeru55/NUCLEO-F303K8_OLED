/*
 * SSD1306_I2C.c
 *
 *  Created on: 2021/09/25
 */

#include "main.h"
#include "SSD1306_I2C.h"

// my sample for OLED SSD1306 unit 128*64 or 128*32

I2C_HandleTypeDef hi2c1;

#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D

///////////////////////////////////////////////////////////////////////////////

static const char OLED_characters[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, //    (Space)
		0x00, 0x00, 0x5F, 0x00, 0x00, //    !
		0x00, 0x07, 0x00, 0x07, 0x00, //    "
		0x14, 0x7F, 0x14, 0x7F, 0x14, //    #
		0x24, 0x2A, 0x7F, 0x2A, 0x12, //    $
		0x23, 0x13, 0x08, 0x64, 0x62, //    %
		0x36, 0x49, 0x56, 0x20, 0x50, //    &
		0x00, 0x08, 0x07, 0x03, 0x00, //    '
		0x00, 0x1C, 0x22, 0x41, 0x00, //    (
		0x00, 0x41, 0x22, 0x1C, 0x00, //    )
		0x2A, 0x1C, 0x7F, 0x1C, 0x2A, //    *
		0x08, 0x08, 0x3E, 0x08, 0x08, //    +
		0x00, 0x00, 0x70, 0x30, 0x00, //    ,
		0x08, 0x08, 0x08, 0x08, 0x08, //    -
		0x00, 0x00, 0x60, 0x60, 0x00, //    .
		0x20, 0x10, 0x08, 0x04, 0x02, //    /
		0x3E, 0x51, 0x49, 0x45, 0x3E, //    0
		0x00, 0x42, 0x7F, 0x40, 0x00, //    1
		0x72, 0x49, 0x49, 0x49, 0x46, //    2
		0x21, 0x41, 0x49, 0x4D, 0x33, //    3
		0x18, 0x14, 0x12, 0x7F, 0x10, //    4
		0x27, 0x45, 0x45, 0x45, 0x39, //    5
		0x3C, 0x4A, 0x49, 0x49, 0x31, //    6
		0x41, 0x21, 0x11, 0x09, 0x07, //    7
		0x36, 0x49, 0x49, 0x49, 0x36, //    8
		0x46, 0x49, 0x49, 0x29, 0x1E, //    9
		0x00, 0x00, 0x14, 0x00, 0x00, //    :
		0x00, 0x40, 0x34, 0x00, 0x00, //    ;
		0x00, 0x08, 0x14, 0x22, 0x41, //    <
		0x14, 0x14, 0x14, 0x14, 0x14, //    =
		0x00, 0x41, 0x22, 0x14, 0x08, //    >
		0x02, 0x01, 0x59, 0x09, 0x06, //    ?
		0x3E, 0x41, 0x5D, 0x59, 0x4E, //    @
		0x7C, 0x12, 0x11, 0x12, 0x7C, //    A
		0x7F, 0x49, 0x49, 0x49, 0x36, //    B
		0x3E, 0x41, 0x41, 0x41, 0x22, //    C
		0x7F, 0x41, 0x41, 0x41, 0x3E, //    D
		0x7F, 0x49, 0x49, 0x49, 0x41, //    E
		0x7F, 0x09, 0x09, 0x09, 0x01, //    F
		0x3E, 0x41, 0x41, 0x51, 0x73, //    G
		0x7F, 0x08, 0x08, 0x08, 0x7F, //    H
		0x00, 0x41, 0x7F, 0x41, 0x00, //    I
		0x20, 0x40, 0x41, 0x3F, 0x01, //    J
		0x7F, 0x08, 0x14, 0x22, 0x41, //    K
		0x7F, 0x40, 0x40, 0x40, 0x40, //    L
		0x7F, 0x02, 0x1C, 0x02, 0x7F, //    M
		0x7F, 0x04, 0x08, 0x10, 0x7F, //    N
		0x3E, 0x41, 0x41, 0x41, 0x3E, //    O
		0x7F, 0x09, 0x09, 0x09, 0x06, //    P
		0x3E, 0x41, 0x51, 0x21, 0x5E, //    Q
		0x7F, 0x09, 0x19, 0x29, 0x46, //    R
		0x26, 0x49, 0x49, 0x49, 0x32, //    S
		0x03, 0x01, 0x7F, 0x01, 0x03, //    T
		0x3F, 0x40, 0x40, 0x40, 0x3F, //    U
		0x1F, 0x20, 0x40, 0x20, 0x1F, //    V
		0x3F, 0x40, 0x38, 0x40, 0x3F, //    W
		0x63, 0x14, 0x08, 0x14, 0x63, //    X
		0x03, 0x04, 0x78, 0x04, 0x03, //    Y
		0x61, 0x59, 0x49, 0x4D, 0x43, //    Z
		0x00, 0x7F, 0x41, 0x41, 0x41, //    [
		0x02, 0x04, 0x08, 0x10, 0x20, //    "\"
		0x00, 0x41, 0x41, 0x41, 0x7F, //    ]
		0x04, 0x02, 0x01, 0x02, 0x04, //    ^
		0x40, 0x40, 0x40, 0x40, 0x40, //    _
		0x00, 0x03, 0x07, 0x08, 0x00, //    `
		0x20, 0x54, 0x54, 0x38, 0x40, //    a
		0x7F, 0x28, 0x44, 0x44, 0x38, //    b
		0x38, 0x44, 0x44, 0x44, 0x28, //    c
		0x38, 0x44, 0x44, 0x28, 0x7F, //    d
		0x38, 0x54, 0x54, 0x54, 0x18, //    e
		0x00, 0x08, 0x7E, 0x09, 0x02, //    f
		0x0C, 0x52, 0x52, 0x4A, 0x3C, //    g
		0x7F, 0x08, 0x04, 0x04, 0x78, //    h
		0x00, 0x44, 0x7D, 0x40, 0x00, //    i
		0x20, 0x40, 0x40, 0x3D, 0x00, //    j
		0x7F, 0x10, 0x28, 0x44, 0x00, //    k
		0x00, 0x41, 0x7F, 0x40, 0x00, //    l
		0x7C, 0x04, 0x78, 0x04, 0x78, //    m
		0x7C, 0x08, 0x04, 0x04, 0x78, //    n
		0x38, 0x44, 0x44, 0x44, 0x38, //    o
		0x7C, 0x18, 0x24, 0x24, 0x18, //    p
		0x18, 0x24, 0x24, 0x18, 0x7C, //    q
		0x7C, 0x08, 0x04, 0x04, 0x08, //    r
		0x48, 0x54, 0x54, 0x54, 0x24, //    s
		0x04, 0x04, 0x3F, 0x44, 0x24, //    t
		0x3C, 0x40, 0x40, 0x20, 0x7C, //    u
		0x1C, 0x20, 0x40, 0x20, 0x1C, //    v
		0x3C, 0x40, 0x30, 0x40, 0x3C, //    w
		0x44, 0x28, 0x10, 0x28, 0x44, //    x
		0x4C, 0x50, 0x50, 0x50, 0x3C, //    y
		0x44, 0x64, 0x54, 0x4C, 0x44, //    z
		0x00, 0x08, 0x36, 0x41, 0x00, //    {
		0x00, 0x00, 0x77, 0x00, 0x00, //    |
		0x00, 0x41, 0x36, 0x08, 0x00, //    }
		0x02, 0x01, 0x02, 0x04, 0x02, //    ~
		};

uint16_t _address = 0x78;
// char _address = 0x3c; // for Arduino

uint8_t I2C_DATA[2]; // max character width 5dot + data

void OLED_command(char command) {
	I2C_DATA[0] = 0;
	I2C_DATA[1] = command;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)_address,  (uint8_t *)I2C_DATA, 2, 300);
}

void OLED_data(char data) {
	I2C_DATA[0] = 0x40;
	I2C_DATA[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)_address, (uint8_t *) I2C_DATA, 2, 300);
}

void OLED_Init() {
	OLED_command(OLED_DISPLAYOFF);         // 0xAE
	OLED_command(OLED_SETDISPLAYCLOCKDIV); // 0xD5
	OLED_command(0x80);                    // the suggested ratio 0x80
	OLED_command(OLED_SETMULTIPLEX);       // 0xA8

#if OLED_HEIGHT == 32
	OLED_command(0x1F);
#elif OLED_HEIGHT == 64
	OLED_command(0x7F);
	#endif

	OLED_command(OLED_SETDISPLAYOFFSET);   // 0xD3
	OLED_command(0x0);                     // no offset
	OLED_command(OLED_SETSTARTLINE | 0x0); // line #0
	OLED_command(OLED_CHARGEPUMP);         // 0x8D
	OLED_command(0xAF);
	OLED_command(OLED_MEMORYMODE); // 0x20
	OLED_command(0x00);            // 0x0 act like ks0108
	OLED_command(OLED_SEGREMAP | 0x1);
	OLED_command(OLED_COMSCANDEC);
	OLED_command(OLED_SETCOMPINS); // 0xDA

#if OLED_HEIGHT == 32
	OLED_command(0x02);
#elif OLED_HEIGHT == 64
OLED_command(0x12);
#endif

	OLED_command(OLED_SETCONTRAST); // 0x81

#if OLED_HEIGHT == 32
	OLED_command(0x8F);
#elif OLED_HEIGHT == 64
	OLED_command(0x9F);
#endif

	OLED_command(OLED_SETPRECHARGE); // 0xd9
	OLED_command(0xF1);
	OLED_command(OLED_SETVCOMDETECT); // 0xDB
	OLED_command(0x40);
	OLED_command(OLED_DISPLAYALLON_RESUME); // 0xA4
	OLED_command(OLED_NORMALDISPLAY);       // 0xA6
	OLED_command(OLED_DISPLAYON);           //--turn on oled panel
}

void OLED_write_bit(char ch, int x, int y) {
	OLED_command(0x20);            //
	OLED_command(0x02);            // Page addressing mode
	OLED_command(y + 0xB0);        // Set page start addres
	OLED_command(x & 0x0F);        // Set lower column start address
	OLED_command(0x10 + (x >> 4)); // Set higher column start address

	OLED_data(ch);
}

void OLED_clear() {
	uint8_t x = 0;
	uint8_t y = 0;
	for (x = 0; x < 128; x++) {
		for (y = 0; y < OLED_HEIGHT / 8; y++) {
			OLED_write_bit(0, x, y);
		}
	}
}

// x: 0 to 127
// y: 0 to 3 or 7

void OLED_char6(char ch, int x, int y) {

	int table_offset = (ch - ' ') * 5;

	for (int i = 0; i < 5; i++) {
		OLED_write_bit(OLED_characters[i + table_offset], x + i, y);
	}

}

void OLED_string6(char *str, int x, int y) {

	int pos = 0;
	int startx = x;
	int starty = y;
	while (str[pos] != '\0') {
		OLED_char6(str[pos], startx, starty);
		if (startx >= 123) {
			starty++;
			startx = 0;
		} //wrap around
		startx += 6;
		pos += 1;
	}
}
