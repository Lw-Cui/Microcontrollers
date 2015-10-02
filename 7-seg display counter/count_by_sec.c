#include <reg51.h>
#include <intrins.h>
#define MS 100	  // MS decides show time of each digital

unsigned char code digit[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,  0x7f, 0x6f
};

void delay_ms(unsigned int time) {
	int i = 110;
	for (; time > 0; time--)
		for (; i > 0; i--);
}

void main() {
	unsigned int count = 0, delay = 0;
	unsigned char status = 1; 
	while (1) {
		//delay decides show time of each number
		for (delay = 300; delay > 0; delay--) {
			P1 = status;
			status = _crol_(status, 1);
			if (status > 1 << 3)
				status = 1;

			switch (status) {
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
			P0 = 0;			 
		}
		if (++count > 9999)
			count = 0;
	}
}
