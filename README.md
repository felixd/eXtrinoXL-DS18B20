# eXtrinoXL DS18B20
Obsługa czujnika temperatury DS18B20 podłączonego do płytki eXtrinoXL. 
Wyniki wyświetlane są na ekranie LCD 16x2 HD44780.

## Instalacja
Po pobraniu repozytorium odpal poniższy skrypt w celu pobrania wymagancyh bibliotek

```
$ ./get_libs.sh
```

## Narzędzia

 * Edytor: [Visual Studio Code](https://code.visualstudio.com/) + [PlatformioIO](http://platformio.org/)
 * Formatowanie kodu: **clang-format**
 * Kodowanie: **UTF-8**
 * Znak końca linii: **Unix LF**

 ## Uwagi do Platformio IO / VSCode

### Kompliacja i programowanie: avrdude, Platformio IO i FLIP (FLIP2)
Upload pliku HEX odbywa się za pomocą oprogramowania FLIP (ang. FLexible In-system Programmer) firmy Atmel wbudowanego w bootloader XMEGA128A3U. Obsługiwany jest przez `avrdude` a uruchamiany poniszym poleceniem:

```./upload.sh```

Patrz plik [upload.sh](upload.sh)

### IntelliSense i <avr/io.h> a xmega128a3u
W celu poprawnego działania **IntelliSense** w **VSCode** upewnij się, że plik `.vscode/c_cpp_properties.json` w sekcji `defines` ma dokładnie tak samo określony rodzaj mikrokontrolera:

```
            "defines": [
                "__AVR_ATxmega128A3U__",
```

Więcej informacji pod linkiem: https://github.com/Microsoft/vscode-cpptools/issues/690

## Autorzy
* Paweł 'felixd' Wojciechowski - <http://www.konopnickiej.com>

## Źródła
* [Elektroda.pl XMEGA 1Wire DS18B20 HD44780 podstawy](https://www.elektroda.pl/rtvforum/viewtopic.php?p=16457302#16457302)