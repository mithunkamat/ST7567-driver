/*
 * ST7567.h
 *
 *  Created on: 22/07/2016
 *      Author: fleite
 */

#ifndef ST7567_H_
#define ST7567_H_

#include "ST7567Commands.h"
#include "Font.h"
#include "GEDIFont/LucidaConsole-10pt.h"
#include "ext_drivers/spi/include/mq55_spi.h"
#include "ext_drivers/gpio/include/mq55_gpio.h"

void ST7567init(void);
void ST7567reset(void);
void ST7567printDataBegin(void);
void ST7567printDataEnd(void);
void ST7567sendData(char data);
void ST7567sendCommand(char command);
void ST7567_clear(void);
void ST7567_write_character(unsigned short x, unsigned short y, int character, const Font *font, unsigned char backgroundColor, unsigned char foregroundColor);
unsigned long ST7567_write_string(unsigned short x, unsigned short y, const char *text, const Font *font, unsigned short backgroundColor, unsigned short foregroundColor);


#endif /* ST7567_H_ */
