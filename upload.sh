#!/bin/bash

# Szybki skrypt na upload, zanim rozpracowane zostanie Platformio IO
# Wymagana wersja avrdude >= 6.1 (Obsługa FLIP2 - XMEGA)

# Czyszczenie środowiska
pio run -t clean
# Kompilowanie programu
pio run
# AVRDUDE firmware.hex upload
avrdude -c flip2 -U application:w:.pioenvs/avr_xmega/firmware.hex:i -p x128a3u 
