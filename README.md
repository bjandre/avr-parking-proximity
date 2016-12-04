Code and electrical schematic for a garage parking proximity meter.

# Hardware:
* MCU - Atmel AVR attiny2313
* Ultrasonic distance sensor - MaxBotix LV-MaxSonar-EZ
* Adafruit LIPO/Li-Ion battery charging circut

# source
* 3rd-party/ - 3rd-party dependancies

* utils/ - break out reusable code for bit operations and led manipulation, with unit tests

* sonar-pwm-3led/ - receive sonar signal via a pulse width signal, change LED color depending on distance.

* sonar-usart-3led-sleep/ - receive sonar signal over serial signal, change LED color depending on distance, sleep mcu when readings are stable, periodically wake to detect changes.

# External dependencies

Build and flash requires avr-gcc, avr-libc and avrdude

Building on macOS with homebrew requires:

* avrdude
* osx-cross/avr/avr-gcc
* osx-cross/avr/avr-libc
* osx-cross/avr/avr-binutils

cmocka is used for C unit testing. Source is included in this repo,
and can be built with:

    cd 3rd-party
    make cmocka
    
Circuit schematic requires [KiCad](http://kicad-pcb.org/)

# TODO
* disable sonar ranging and periodically wake up to preserve battery.
