#define START_F 3
#define RESET_F 4
#define DIPVALS 5

/*	flag_reg:
Bits 0-2:	countdown
Bit 3:		timer start flag
Bit 4:		timer reset flag
Bits 5-7:	dip input value	
*/
uint8_t flag_reg = 0x00;

int8_t flagSet(int8_t bitPos) {
	if (flag_reg & (1<<bitPos)) return 1;
	else return 0;
}

void setFlag(int8_t bitPos) {
	flag_reg |= (1<<bitPos);
}

void clearFlag(int8_t bitPos) { 
	flag_reg &= ~(1<<bitPos);
}
