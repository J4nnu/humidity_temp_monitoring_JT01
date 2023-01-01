/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

//i2c honeywell anturin osote 0x27
//sda D4, scl D5
// i2c rate 100Khz
// i2c.write(osoite,tallennuspaikka,tavujen maara); Herätetään anturi, anturi suorittaa mittauksen ja tallentaa arvot anturin muistiin.
//addres = 0x27 = 0010 0111 = 39
//osoitteesta 7ensimmäistä bittiä varattu osoitteelle, viimeinen bitti read/write
//D0 ja D1 varataan WLAN modulille. 

#include "mbed.h"
#include <cmath>
#include <cstdio>

I2C i2c(D4,D5);//SDA,SCL
int addr = 39<<1; //Anturin osoite 0x27=dec 39.


void sensorMeasure();
void sensorError();


char data[4]={0,0,0,0}; //Anturista noudetut tiedot tallennetaan.

float temp=0;
float humidity=0;
char sensorStatus=0;

int main()
{
    i2c.frequency(100000);//i2c väylän taajuus/nopeus
    printf("Ohjelman alku.\n");

    while (true) {
        sensorMeasure();
        // printf("humidity %i \n", sensorData[0]);
        // printf("temperature %i \n", sensorData[1]);
        ThisThread::sleep_for(5s);
    }
}

void sensorMeasure(){
    
    unsigned short rawtemp=0;
    unsigned short rawhumid=0;

    i2c.write(addr,data,0);//Measure request
    ThisThread::sleep_for(50ms);
    i2c.read(addr,data,4);//fetch measurements to "data" array

    sensorStatus=data[0]>>6;//Virheentarkistus, jos status eri kun 0x00.
    if (sensorStatus!=0x00) {
        sensorError();
    }

    rawtemp=data[2]<<6|data[3]>>2;//2lsb 0 tai ei kaytossa. Siirretaan toinen rypas 2 askelta oikealle, toinen 6askelta vasemmalle niin kayttamattomat bitit jaa ulkopuolelle, 
    rawhumid=data[0]<<8|data[1];//2msb bittia status. Jos kaikki ok, molemmat on 0. tehdaan virheenhallinta.

    temp=(float)rawtemp/16382*165-40; //Tarkka lampotila float muodossa
    //Seuraavat rivit mahdollistavat lampotilan tulostamisen.
    int tempint=temp;
    int tempDesimal=((int)(1000*temp))%1000; //kun arvo kerrotaan ensin 1000, ja sen jalkeen otetaan jakojaannos 1000, jaljelle jaa alkuperaiset desimaalit.
    printf("Lampotila on: %i.%03i C\n",tempint,tempDesimal); //%03i tulostaa 3 merkkiä ja ottaa 0 mukaan. Eli 012=012 eikä 12. Tama arvo lisataan pisteella tulostuksen peraan ja saadaan xx.xxx tulostus.

    humidity=(float)rawhumid/16382*100; //tarkka ilmankosteus float muodossa.
    int humint=humidity;
    int humDesimal=((int)(1000*humidity))%1000;
    printf("Ilmankosteus on: %i.%03i %%\n",humint,humDesimal);
}

void sensorError(){
    if (sensorStatus==0x00){
        printf("Sensor ok\n");
    }

    else if(sensorStatus==0x01){
    printf("Data fetched already, New fetch\n");
    sensorMeasure();
    }

    else if(sensorStatus==0x02){
        printf("Sensor in Command mode, boot system.\n");
        ThisThread::sleep_for(5s);
    }
    else {
    printf("\n");
    }

}