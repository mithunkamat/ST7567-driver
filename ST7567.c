/*
 * ST7567.c
 *
 *  Created on: 22/07/2016
 *      Author: fleite
 */

#include "ST7567.h"


#define ST7567_TFT_WIDTH 128

void ST7567init(void){
	ST7567reset();
	ST7567sendCommand(ST7567_EXIT_SOFTRST);
	ST7567sendCommand(ST7567_BIAS_1_7);
	ST7567sendCommand(ST7567_DISPON);
	ST7567sendCommand(SSD1305_MAPCOL0);
	ST7567sendCommand(ST7567_SETCOMREVERSE);
	ST7567sendCommand(ST7567_REG_RES_RR1);
	ST7567sendCommand(ST7567_SETEV);
	ST7567sendCommand(0x08);
	ST7567sendCommand(ST7567_POWERCTRL);
	ST7567sendCommand(ST7567_SETSTARTLINE);
	ST7567sendCommand(ST7567_SETPAGESTART);
	ST7567sendCommand(ST7567_SETCOLH);
	ST7567sendCommand(ST7567_SETCOLL);
	ST7567sendCommand(ST7567_DISPINVERSE);
	ST7567sendCommand(ST7567_DISPON);
	ST7567sendCommand(ST7567_DISPRAM);
}

void ST7567reset(void){
	int j;
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV0, 17, 1, 0);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV1, 30, 1, 0);
	for(j=0;j<50000;j++);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV0, 17, 1, 1);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV1, 30, 1, 1);
}

void ST7567printDataBegin(void){
	//SET A0 to send Data
	mq55_spi_ss0_enable(MQ55_SPI_DEV1);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV0, 16, 1, 1);

	/* Outro LCD na SPI2 */
	mq55_spi_ss0_enable(MQ55_SPI_DEV2);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV1, 31, 1, 1);
}

void ST7567printDataEnd(void){
	mq55_spi_ss0_disable(MQ55_SPI_DEV1);

	/* Outro LCD na SPI2 */
	mq55_spi_ss0_disable(MQ55_SPI_DEV2);

}

void ST7567sendData(char data){
	char temp = data;
	mq55_spi_transmit(MQ55_SPI_DEV1, &temp, 1);

	/* Outro LCD na SPI2 */
	temp = data;
	mq55_spi_transmit(MQ55_SPI_DEV2, &temp, 1);
}
void ST7567sendCommand(char command){
	char temp = command;
	mq55_spi_ss0_enable(MQ55_SPI_DEV1);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV0, 16, 1, 0);
	mq55_spi_transmit(MQ55_SPI_DEV1, &temp, 1);
	mq55_spi_ss0_disable(MQ55_SPI_DEV1);

	/* Outro LCD na SPI2 */
	temp = command;
	mq55_spi_ss0_enable(MQ55_SPI_DEV2);
	mq55_gpio_write_bit_pattern(MQ55_GPIO_DEV1, 31, 1, 0);
	mq55_spi_transmit(MQ55_SPI_DEV2, &temp, 1);
	mq55_spi_ss0_disable(MQ55_SPI_DEV2);
}

void ST7567_clear(void){
	int page, j;
	int col = 0;
	for(page = 0; page < 4; page++){
		ST7567sendCommand(ST7567_SETPAGESTART+page);
		ST7567sendCommand(ST7567_SETCOLL + (col & 0x0f));
		ST7567sendCommand(ST7567_SETCOLH + (col >> 4));
		ST7567printDataBegin();
		for(j=0;j<132;j++){
				ST7567sendData(0xFF);
				}
		ST7567printDataEnd();
	}
}

void ST7567_write_character(unsigned short x, unsigned short y, int character, const Font *font, unsigned char backgroundColor, unsigned char foregroundColor)
{
	char column = 0, row = 0; /* Loop indices */
	unsigned char width = 0 ;
	unsigned short position = 0;
	unsigned char index = 0;
	unsigned char bytesByColumn = 0;
	unsigned char page, col, data;

	page = y >> 3;
	col = x;

	ST7567sendCommand(ST7567_SETPAGESTART+page);
	ST7567sendCommand(ST7567_SETCOLL + (col & 0x0f));
	ST7567sendCommand(ST7567_SETCOLH + (col >> 4));
	ST7567printDataBegin();
	
	bytesByColumn = font->descriptorArray[character - font->startCharacter].width / 8;

	if(font->descriptorArray[character - font->startCharacter].width % 8)
		bytesByColumn += 1;

	for(col = 0; col < font->descriptorArray[character - font->startCharacter].width; col++){
		position = font->descriptorArray[character - font->startCharacter].position + (col * bytesByColumn);
		data = font->bitmapArray[position];
		ST7567sendData(~data);
	}

	ST7567printDataEnd();

}

unsigned long ST7567_write_string(unsigned short x, unsigned short y, const char *text, const Font *font, unsigned short backgroundColor, unsigned short foregroundColor)
{
  unsigned long count = 0;

  while(*text)
  {
    if((x + font->descriptorArray[text[0] - font->startCharacter].width) > ST7567_TFT_WIDTH) /* Verifico se e possivel imprimir o caracter */
      return count;  // number of characters printed

    ST7567_write_character(x, y, *text, font, backgroundColor, foregroundColor); /* Envia o caracter. */

    x = x + font->descriptorArray[text[0] - font->startCharacter].width; /* Desloco a posicao para o proximo caracter. */

    text++; /* Seleciona o proximo caracter. */

    count++;
  }

  return count;  /* Number of characters printed */
}
