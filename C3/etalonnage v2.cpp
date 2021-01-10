
#include "mbed.h"
#include "m3pi.h"
#include "string.h"
#include "PowerControl/EthernetPowerControl.h"

m3pi m3pi;                                  // Initialise the m3pi

Serial pc(USBTX, USBRX);                    // For debugging and pc messages, uses commented out to prevent hanging
Timer t1;

DigitalIn bouton(p21, PullUp);
BusOut leds(LED1, LED2, LED3, LED4);

unsigned short tab[5];
#define seuil(x) (x > 400 ? 1 : 0)
char msg[20];
char mess1[6];
char w;
char vse;
unsigned char state;

char seuillage(){
    short i;
    char x = 0;
    for(i = 0; i < 5; i++){
        x = x + (seuil(tab[i]) ? 0x20 : 0x00);
        x = x >> 1;
    }
    return x;
}

void affcapt(unsigned char x, char *p){
    short i;
    for(i = 0; i < 5; i++){
        p[i] = (x&0x01) ? '1' : '0';
        x = x >> 1;
    }
    p[i] = 0;
}

void affiche(int v, int ligne){
    sprintf(msg, "%4u", v);
    m3pi.locate(0, ligne);
    m3pi.print(msg, strlen(msg));
}

void autom1(){
    static unsigned char vetat = 0;
    switch(vetat){
        case 0:
            m3pi.forward(0.25);
            vetat = 1;
            break;
        case 1:
            vse = seuillage();
            if(vse == 0x00){
                vetat = 2;   
            }
            break;
        case 2:
            vse = seuillage();
            if(vse != 0x00){
                vetat = 3;
                t1.reset();
                t1.start();
            }
            break;
        case 3:
            vse = seuillage();
            if(vse == 0x00){
                vetat = 4;   
            }
            break;
        case 4:
            vse = seuillage();
            if(vse != 0x00){
                t1.stop();
                m3pi.stop();
                affiche(t1.read_us(), 0);
            }
            break;
        default:
            break;
    }
    state = vetat;
}

int main() {
    PHY_PowerDown();
    m3pi.sensor_auto_calibrate();
    m3pi.cls();
    wait(1.);
    
    while(1) {
        m3pi.calibrated_sensors(tab);
        autom1();
    }
}