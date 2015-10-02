#include <reg51.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define ushort unsigned short

#define MS 10    	  // MS decides show time of each digital
#define INITIAL_NUM 15536	    //the initial value of count 50ms: 65536 - 50000 = 15536
#define TIME_OF_50MS 20

uint count = 0;
ushort run = 1;
sbit k5 = P3 ^ 5;
sbit k1 = P1 ^ 4;
sbit k2 = P1 ^ 5;

uchar code digit[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,  0x7f, 0x6f
};
 
void counter() interrupt 1 {
	static uint num = 0;
	TH0 = INITIAL_NUM / 256;
	TL0 = INITIAL_NUM % 256;

	if (++num == TIME_OF_50MS) {
		num = 0;
		if (run && count++ > 9999)
			count = 0;
	}
}

void init() {
	TMOD = 0x01;   	//method 1, 16-bit counter
	TH0 = INITIAL_NUM / 256;
	TL0 = INITIAL_NUM % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void delay_ms(uint time) {
	uint i = 110;
	for (; time > 0; time--)
		for (; i > 0; i--);
}

uchar next_status(uchar s) {
	uchar high = 0xF0 & s, low = 0x0F & s;
	low = _crol_(low, 1);
	if (low > 1 << 3)
		low = 1;
	s = low + high;
	return s;
}



void scan_key() {
	if (!k5) {
		delay_ms(MS);
		if (!k5) {
			while (!k5);
			count = 0;
		}
	}

	if (!k1) {
		delay_ms(MS);
		if (!k1) {
			while (!k1);
			run = 0;
		}
	}

	if (!k2) {
		delay_ms(MS);
		if (!k2) {
			while (!k2);
			run = 1;
		}
	}
		
}

void display() {
	static uchar pos = 0xF1;
	P1 = pos;
	pos = next_status(pos);
	switch (pos & 0x0F) {
	case 1:
		P0 = digit[count / 1000];
		break;
	case 2:
		P0 = digit[count % 10];
		break;
	case 4:
		P0 = digit[count / 10 % 10];
		break;
	case 8:
		P0 = digit[count / 100 % 10];
		break;
	default:
		break;
	}	
	delay_ms(MS);	
	//avoid conflict
	P0 = 0;		
}

void main() { 
	init();

	while(1) {
		scan_key();	
		display();
	}
}
