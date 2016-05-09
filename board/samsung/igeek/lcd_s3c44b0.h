/*
 * lcd.h
 *
 *  Created on: May 4, 2016
 *      Author: darkspr1te
 */
/*
#include "../lcd/fpboard.h"
#include "../lib/serial.h"
#include "../lib/timer.h"
*/
#ifndef SPIBASH_H_
#define SPIBASH_H_
#define PORTC_LCD_DATA (*((volatile long *)0x01d20014)) /*PDATC Data Port C*/
#define PORTC_LCD_CONFIG_ADDR (*((volatile long *)0x01d20010)) /*PCONC (Port C Control*/
#define PORTC_LCD_CONFIG 0x55555555 /*All outputs */

#define GPIOC0 0x1
#define GPIOC1 0x2
#define GPIOC2 0x4
#define GPIOC3 0x8
#define GPIOC4 0x10
#define GPIOC5 0x20
#define GPIOC6 0x40
#define GPIOC7 0x80
#define GPIOC8 0x100
#define GPIOC9 0x200
#define GPIOC10 0x400


#define RESET_PIN GPIOC2
#define CLK_PIN GPIOC0
#define SDO_PIN GPIOC1
#define CS_PIN GPIOC9
#define RS_PIN GPIOC8

#define iiscon ((uint32_t volatile* const) 0x01d18000) /* IISCON (IIS Control) */
#define iismod ((uint32_t volatile* const) 0x01d18004) /* IISMOD (IIS Mode) */
#define iispsr ((uint32_t volatile* const) 0x01d18008) /* IISPSR (IIS Prescaler) */
#define iisfifcon ((uint32_t volatile* const) 0x01d1800c) /* IISFIFCON (IIS FIFO Control) */
#define iisfif ((uint32_t volatile* const) 0x01d18010) /* IISFIF (IIS FIFO Entry) */

#define ISSCON_CONFIG 0x1
#define ISSMOD_CONFIG 0x128
#define IISPSR_CONFIG 0x0
#define IISFCON_CONFIG 0x0



/*==================
 * LCD COMMANDS
 */
#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0 //read-modify-write
#define CMD_RMW_CLEAR 0xEE //END command
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0





void none_void(void);
void drawchar(int *buff, int x, int line, int c);
void init_port(void);
void draw_string(char *buff,int *buffer_data, int line);
void shiftOut(int dataPin, int clockPin, int val);
void reset_lcd(void);
void send_cmd(int cmd);
void send_data(int cmd);
void lcd_init(void);
void clear_screen(void);
void empty_buffer(int *buffer_data);
void write_buffer(int *buffer_data);
void display_picture(int pic[]);
void Initial_Dispay_Line(int line);
void write_num(int *n,int k,int station_dot);
void Set_Page_Address(unsigned char add);
void Set_Column_Address(unsigned char add);
#endif /* SPIBASH_H_ */
