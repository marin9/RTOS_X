#include "device.h"

#define VCC	19
#define CS	21
#define CK	20
#define SI	16
#define SO	26


void spi_init(){
	gpio_open(VCC, GP_OUT);
	gpio_open(CS, GP_OUT);
	gpio_open(SO, GP_IN);
	gpio_open(SI, GP_OUT);
	gpio_open(CK, GP_OUT);

	gpio_write(VCC, GP_HIGH);
	gpio_write(CS, GP_HIGH);
	gpio_write(SI, GP_LOW);
	gpio_write(CK, GP_LOW);
}

void spi_begin(){
	gpio_write(CS, GP_LOW);
	timer_wait(1);
}

void spi_end(){
	timer_wait(1);
	gpio_write(CS, GP_HIGH);
}

void spi_sendByte(char byte){
	int i;
	for(i=7;i>=0;--i){
		gpio_write(CK, GP_LOW);
		gpio_write(SI, byte&(1<<i));
		timer_wait(1);
		gpio_write(CK, GP_HIGH);
		timer_wait(1);
	}
}

char spi_readByte(){
	int i;
	unsigned char byte=0;
	char bit[8];

	for(i=7;i>=0;--i){
		gpio_write(CK, GP_LOW);
		timer_wait(1);
		bit[i]=gpio_read(SO);
		timer_wait(1);
		gpio_write(CK, GP_HIGH);
		timer_wait(1);
	}

	gpio_write(CK, GP_LOW);
	timer_wait(1);

	for(i=0;i<8;++i){
		byte=byte|((0x01&bit[i])<<i);
	}

	return byte;
}