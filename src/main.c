/*
  Data: 28.01.2018
  Paweł 'felixd' Wojciechowski
  Outsourcing IT - Konopnickiej.Com
  http://www.konopnickiej.com

*/

#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <hd44780.h>

int main(void)
{
	// Konfiguracja przycisku FLIP
	PORTE.DIRCLR = PIN5_bm;				 // pin E5 jako wejście
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc; // podciągnięcie do zasilania

	// Inicjalizacja sterownika LCD
	LcdInit();

	Lcd("    DS18B20");
	Lcd2; // Przejście do drugiej linii
	Lcd("Konopnickiej.Com");

	// Deklarujemy zmienną służącą jako licznik
	uint8_t licznik = 0;

	while (1)
	{
		if (!(PORTE.IN & PIN5_bm))
		{					 // jeżeli przycisk FLIP jest wciśnięty
			licznik++;		 // zwiększ licznik o 1
			LcdClear();		 // wyczyśc wyświetlacz
			Lcd("Licznik:"); // wyświetlenie napisu
			Lcd2;			 // przejście do drugiej linii
			LcdDec(licznik); // wyświetlenie zmiennej liczbowej
			_delay_ms(100);  // czekanie 100ms
		}
	}
}
