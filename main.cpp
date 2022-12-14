/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

//i2c osote 0x27
//sda D4, scl D5
// i2c rate 100Khz
// i2c.write(osoite,tallennuspaikka,tavut); Herätetään anturi, anturi suorittaa mittauksen ja tallentaa muistiin.
//adres = 0x27 = 0010 0111 = 39
//osoitteesta 7ensimmäistä bittiä varattu osoitteelle, viimeinen bitti read/write
#include "mbed.h"
I2C i2c(D4,D5);//SDA,SCL

void sensorMeasure();
int sensorData[]={0,0};
int addr = 39; //Anturin osoite 0x27=dec 39.
int fetch = 79; //Bitit siirretty ja osoite muokattu tiedon noutoa varten
int measure=78; //Bitit siirretty ja osoite muokattu anturin mittausta varten. 
char data[4]={0,0,0,0}; //Anturista noudetut tiedot tallennetaan.

int main()
{
    i2c.frequency(100000);//i2c väylän taajuus/nopeus
    printf("Ohjelman alku.\n");
        
    
    // i2c.write(measure,data,1,true);
    // i2c.write(addr,data,1);
    // ThisThread::sleep_for(5ms);
    // i2c.read(fetch,data,4);
  

    while (true) {
     

        sensorMeasure();
        printf("humidity %i \n", (int)sensorData[0]);
        printf("pemperature %i \n", (int)sensorData[1]);
        float lampoa=sensorData[1];
        // i2c.write(measure,data,1);
        // ThisThread::sleep_for(500ms);
        // i2c.read(fetch,data,4);
        

        // char ekat =data[0];
        // char toka =data[1];
        // char kolmas =data[2];
        // char neljas =data[3];
        // printf("eka = %d\n", ekat);
        // printf("toka = %d\n", toka);
        // printf("kolmas = %d\n", kolmas);
        // printf("neljas = %d\n", neljas);
        ThisThread::sleep_for(5000ms);

    }
}

void sensorMeasure(){
    i2c.write(measure,data,1);//Measure request
    ThisThread::sleep_for(30ms);
    i2c.read(fetch,data,4);//fetch measurements to "data" array
    sensorData[0]=data[0]<<8|data[1];//save humidity data to array [0]
    sensorData[1]=(data[2]<<6|data[3]>>2);
    float contemp=sensorData[1];
}