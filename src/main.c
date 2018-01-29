#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <ds18b20.h>

#define UART_BAUD_RATE 2400
#include <uart.h>

int main(void) {
	char printbuff[100];
	double d = 0;

	//init UART
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	//init interrupt
	sei();

	for (;;) {
		d = ds18b20_gettemp();

		dtostrf(d, 10, 3, printbuff);
		_delay_ms(500);
	}
	
	return 0;
}
