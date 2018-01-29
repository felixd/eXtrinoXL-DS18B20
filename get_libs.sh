
#!/bin/bash

# Author:   Paweł 'felixd' Wojciechowski
# Company:  (c) 2018 - Outsourcing IT - Konopnickiej.Com
# WWW:      http://www.konopnickiej.com

cd lib

# HD44780 LCD library
if [ -d "hd44780" ]; then 
 cd hd44780
 git pull
 cd ..
else
 git clone https://github.com/eXtrinoXL/hd44780
fi

# DS18B20 thermometer library
# if [ -d "ds18b20" ]; then 
#  cd ds18b20
#  git pull
#  cd ..
# else
#  git clone https://github.com/eXtrinoXL/ds18b20
# fi
