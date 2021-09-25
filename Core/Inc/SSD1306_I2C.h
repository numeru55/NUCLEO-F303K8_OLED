/*
 * SSD1306_I2C.h
 *
 *  Created on: 2021/09/25
 */

#ifndef SSD1306_I2C_H_
#define SSD1306_I2C_H_

void OLED_command(char command);
void OLED_data(char data);
void OLED_Init();
void OLED_write_bit(char ch, int x, int y);
void OLED_clear();
void OLED_char6(char ch, int x, int y);
void OLED_string6(char *str, int x, int y);

#endif /* SSD1306_I2C_H_ */
