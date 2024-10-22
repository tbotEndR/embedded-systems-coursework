make: main.c
	avr-gcc -O1 -Wall -mmcu=atmega328p main.c -o main.o
	avr-objcopy -O ihex main.o main.hex
	avrdude -c usbasp -p atmega328P -U flash:w:main.hex:i
