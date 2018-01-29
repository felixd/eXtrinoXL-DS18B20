/*
  Data: 29.01.2018
  Paweł 'felixd' Wojciechowski
  Outsourcing IT - Konopnickiej.Com
  http://www.konopnickiej.com

  Poniżej jest delikatnie zmodyfikowany kod który znalazlem pod adresem:
  https://www.elektroda.pl/rtvforum/viewtopic.php?p=16457302#16457302

  Poprawiłem z nim niepoprawne typy zmiennych i złe konwertowania co objawiało się ciekawymi wynikami 
  pokazywanej temperatury.
*/

#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <hd44780.h> //biblioteka Radoslaw Kwiecien - zmodyfikowana przez Leon Instruments

//port i pin do ktorego jest podlaczony czujnik - dla eXtrino to PORTA PIN7_bm (! zworka J80 zalutowana - PullUp)
#define OW_PORT PORTA
#define OW_PIN_bm PIN7_bm

//Funkcja resetuje magistrale i oczekuje na impuls Presence
unsigned char uc1Wire_ResetPulse(void)
{
	unsigned char ucPresenceImpulse; //zmianna do ktorej bedzie wpisywany wynik wyszukiwania obeznosci czujnika

	OW_PORT.DIRSET = OW_PIN_bm; //ustawienie pinu jako wyjscie
	OW_PORT.OUTCLR = OW_PIN_bm; //ustawienie stanu niskiego
	_delay_us(500);				//odczekanie 500us
	OW_PORT.DIRCLR = OW_PIN_bm; //ustawienie pinu jako wejscie
	_delay_us(45);				//odczekanie 45us

	if (!(OW_PORT.IN & OW_PIN_bm))
		ucPresenceImpulse = 1;
	else
		ucPresenceImpulse = 0;

	_delay_us(470); //odczekanie 470us

	return ucPresenceImpulse;
}

//Funkcja wysyla na magistrale pojedynczy bit
void v1Wire_SendBit(char cBit)
{
	OW_PORT.DIRSET = OW_PIN_bm; //ustawienie kierunku pinu na wyjscie
	_delay_us(5);

	if (cBit == 1)
		OW_PORT.DIRCLR = OW_PIN_bm; //zwonienie magistrali, pin na we

	_delay_us(80); //przetrzymanie, wysłanie zera
	OW_PORT.DIRCLR = OW_PIN_bm;
}

//Funkcja odbiera bit z magistrali
unsigned char uc1Wire_ReadBit(void)
{
	unsigned char ucBit;

	OW_PORT.DIRSET = OW_PIN_bm; //kierunek pinu na wyjscie
	_delay_us(2);				//odczekanie 2us
	OW_PORT.DIRCLR = OW_PIN_bm; //zwonienie magistrali
	_delay_us(15);				//odczekanie 15us

	if (OW_PORT.IN & OW_PIN_bm)
		ucBit = 1; //odbior wartosci "1"
	else
		ucBit = 0; //odbior wartosci "0"

	return (ucBit);
}

//Funkcja wysyła bajt na magistrale
void v1Wire_SendByte(char ucByteValue)
{
	unsigned char ucCounter;	 //zmienna licznikowa
	unsigned char ucValueToSend; //wartosc do wyslania

	for (ucCounter = 0; ucCounter < 8; ucCounter++) //wysylanie bitow pojedynczo
	{
		ucValueToSend = ucByteValue >> ucCounter; //przesuniecie bitowe w prawo
		ucValueToSend &= 0x01;					  //skopiowanie bitu
		v1Wire_SendBit(ucValueToSend);			  //wyslanie bitu na magistrale
	}
	_delay_us(100); //odczekanie 100us
}

//Funkcja odbiera bajt z magistrali
unsigned char uv1Wire_ReadByte(void)
{
	unsigned char ucCounter;	  //zmienna licznikowa
	unsigned char ucReadByte = 0; //odczytywana wartosc

	for (ucCounter = 0; ucCounter < 8; ucCounter++) //petla wykonywana 8 razy
	{
		if (uc1Wire_ReadBit()) //odczyt po jednym bicie
			ucReadByte |= 0x01 << ucCounter;
		_delay_us(15); //odczekanie 15us
	}
	return (ucReadByte);
}

int main(void)
{
	unsigned char ucReset;
	uint8_t cTemperatureH = 0, cTemperatureL = 0;
	double fTemperature = 0; //otrzymana temperatura, double
	uint16_t sTemperature;   // uint16_t z racji 12 bitowej rozdzielczości zwracanej temperatury

	LcdInit();				 //inicjalizacja wyswietlacza
	LcdClear();				 //wyczyszczenie wyswietlacza
	Lcd("   eXtrino XL   "); //wyswietlenie napisu
	Lcd2;					 //przejscie do kolejnej lini
	Lcd(" 1-wire DS18B20 "); //wyswietlenie napisu
	_delay_ms(500);			 //odczekanie 10000ms

	while (1) //petla glowna programu
	{
		ucReset = uc1Wire_ResetPulse(); //impuls resetu

		if (ucReset == 1)
		{
			v1Wire_SendByte(0xCC);				//SKIP ROM
			v1Wire_SendByte(0x44);				//CONVERT T
			_delay_ms(750);						//odczekanie 750ms czas konwersji (12bit)
			ucReset = uc1Wire_ResetPulse();		//wyslanie impulsu reset
			v1Wire_SendByte(0xCC);				//SKIP ROM
			v1Wire_SendByte(0xBE);				//READ SCRATCHPAD
			cTemperatureL = uv1Wire_ReadByte(); //odczytanie LSB
			cTemperatureH = uv1Wire_ReadByte(); //odczytanie MSB
			ucReset = uc1Wire_ResetPulse();		//zwolnienie magistrali

			// Domyslnie temperatura zwracana jest w rozdzielczości 12 bitowej.
			fTemperature = (double)((cTemperatureH << 8) + cTemperatureL) / 16;
			dtostrf(fTemperature, 3, 4, sTemperature); // Konwertowanie do formatu nnn.nnnn (string)

			LcdClear();			   //wyczyszczenie wyswietlacza
			LcdBin(cTemperatureH); //wyswietla MSB
			LcdBin(cTemperatureL); //wyswietla LSB
			LcdGoto(0, 1);		   //przejscie do kolejnej linii
			LcdWrite((char *)sTemperature); // Funkcja LcdWrite przyjmuje (char *) jak parametr
			LcdWrite("oC 12bit");
		}
		else
		{
			LcdClear();			 //wyczyszczenie wyswietlacza
			Lcd("BRAK ODCZYTU"); //wyświetlenie napisu
		}

		_delay_ms(1000); // Ponowny odczyt temperatury za 1s
	}
}