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
