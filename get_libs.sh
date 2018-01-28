
#!/bin/bash

# Author:   Paweł 'felixd' Wojciechowski
# Company:  (c) 2018 - Outsourcing IT - Konopnickiej.Com
# WWW:      http://www.konopnickiej.com

cd lib
if [ -d "hd44780" ]; then 
 cd hd44780
 git pull
 cd ..
else
 git clone https://github.com/eXtrinoXL/hd44780
fi
