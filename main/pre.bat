copy .\%1\%1.ino .\%1\%1.cpp
"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avr-g++" -E -std=gnu++11  .\%1\%1.cpp
rm .\%1\%1.cpp
