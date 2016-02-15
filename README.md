Personal avr development projects to get familiar with avr-libc..

# External dependencies #

avr-gcc, avrdude

Building on OS X with homebrew requires:

* avrdude
* osx-cross/avr/avr-gcc
* osx-cross/avr/avr-libc
* osx-cross/avr/avr-binutils

cmocka is used for C unit testing. Source is included in this repo,
and can be built with:

    cd 3rd-party
    make cmocka
    

# Projects #

* single-led - turn on a single led
* single-led-pwm - turn on a single led with pulse width modulation
* three-leds - change colors on an RGB LED
* three-leds-pwm - change colors on an RGB LED using pulse width modulation
* sonar-pwm-3led - use an ultrosonic sonar sending a pulse width signal, change LED color depending on distance.
* sonar-usart-3led - use an ultrosonic sonar sending a serial signal, change LED color depending on distance.
