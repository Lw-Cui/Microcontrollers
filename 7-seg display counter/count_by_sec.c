#include <reg51.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define MS 10    	  // MS decides show time of each digital
#define INITIAL_NUM 15536	    //the initial value of count 50ms: 65536 - 50000 = 15536
#define TIME_OF_50MS 20

uint count = 0;
uchar code digit[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,  0x7f, 0x6f
};
 
void counter() interrupt 1 {
	static uint num = 0;
	TH0 = INITIAL_NUM / 256;
	TL0 = INITIAL_NUM % 256;

	if (++num == TIME_OF_50MS) {
		num = 0;
		if (count++ > 9999)
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
	int i = 110;
	for (; time > 0; time--)
		for (; i > 0; i--);
}

uchar next_status(uchar s) {
	s = _crol_(s, 1);
	if (s > 1 << 3)
		s = 1;
	return s;
}

void main() {
	uint delay = 0;
	uchar pos = 1; 
	init();

	while(1) {
		P1 = pos;
		pos = next_status(pos);
		switch (pos) {
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
}
