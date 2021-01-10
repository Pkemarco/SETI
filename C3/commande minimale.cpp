
#include "mbed.h"
#include "m3pi.h"
#include "string.h"
#include "PowerControl/EthernetPowerControl.h"

m3pi m3pi;                                  // Initialise the m3pi

Serial pc(USBTX, USBRX);                    // For debugging and pc messages, uses commented out to prevent hanging
Timer t1;

unsigned short tab[5];
char msg[6];
char vse;

#define seuil(x) (x > 400 ? 1 : 0)
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

void autom(){
    static unsigned char vetat = 0;
    static unsigned char speed = 0;
    static char initCapt = 0;;

    switch(vetat){
        case 0: // Initialisation
            speed = 0;
            initCapt = seuillage();
            vetat = 1;
            break;
        case 1: // Incrémentation
            speed += 1;
            if(speed > 100){  // Erreur
                m3pi.locate(0,0);
                m3pi.print("Erreur !\0", 9);
                vetat = 3; // Lancera le default
            }
            else{
                vetat = 2;
                m3pi.forward(speed/100.);
                t1.reset();
                t1.start();
            }
            break;
        case 2: // Test
            vse = seuillage();
            if(t1.read_us() > 2000000 && vse == initCapt){
                t1.stop();
                vetat = 1; // On augmente la vitesse
            }
            else if(vse != initCapt){ // Affichage car le robot a bougé
                t1.stop();
                m3pi.stop();
                affcapt(speed, msg);
                m3pi.locate(0,0);
                m3pi.print(msg, strlen(msg));
                vetat = 3; // Lancera le default
            }
            break;
        default: break;
    }   
}

int main() {
    PHY_PowerDown();
    m3pi.sensor_auto_calibrate();
    m3pi.cls();
    wait(1.);
    
    while(1) {
        m3pi.calibrated_sensors(tab);
        autom();
    }
}