/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

//i2c osote 0x27
//sda D4, scl D5
// i2c rate 100Khz
// i2c.write(osoite,tallennuspaikka,tavut); Herätetään anturi, anturi suorittaa mittauksen ja tallentaa muistiin.
//adres = 0x27 = 0010 0111 = 39
//osoitteesta 7ensimmäistä bittiä varattu osoitteelle, viimeinen read/write
#include "mbed.h"
I2C i2c(D4,D5);//SDA,SCL

// Blinking rate in milliseconds
#define BLINKING_RATE     5000ms
int addr = 39; //Anturin osoite 0x27=dec 39. siirretään koska viimeinen bitti fets/ measure
int fetch = 79;
int measure=78;

int main()
{
    i2c.frequency(100000);//i2c väylän taajuus/nopeus
    char data[4]={0,0,0,0};
        printf("ALKU!!");
        
    i2c.start();
    // i2c.write(measure,data,1,true);
    i2c.write(addr,data,1);
    ThisThread::sleep_for(5ms);
    i2c.read(fetch,data,4);
    i2c.stop();

    while (true) {
        // i2c.start();
        // i2c.write(addr,data,1);
        // ThisThread::sleep_for(500ms);
        // i2c.read(addr,data,4);
        // i2c.stop();

        char ekat =data[0];
        char toka =data[1];
        char kolmas =data[2];
        char neljas =data[3];
        printf("eka = %d\n", ekat);
        printf("toka = %d\n", toka);
        printf("kolmas = %d\n", kolmas);
        printf("neljas = %d\n", neljas);
        ThisThread::sleep_for(BLINKING_RATE);

        // int tmp = (char((data[0] << 8) | data[1]) / 256.0);


    }
}
