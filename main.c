/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on March 29, 2016, 3:27 PM
 */

#include "types.h"
#include <p32xxxx.h>

int main()
{
    set_timer();
//  exo1();
    exo2();
//  exo3();
//  exo4();

    return (1);
}

void    exo1()
{
    u8  debounce;

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

    while (1)
    {
        debounce = 0;
        while (PORTDbits.RD8 == 0)
            debounce++;
        if (debounce > 3)
            LATFbits.LATF1 = !LATFbits.LATF1;
    }
}

void    exo2()
{
    u8  debounce;
    u16  timeCount;
    u16  timeCountDefault;

    timeCountDefault = 3906;
    timeCount = timeCountDefault;
    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

     while (1)
     {

        debounce = 0;
        while (PORTDbits.RD8 == 0)
            debounce++;
        if (debounce > 3)
            timeCount /= 2;
        if (timeCount < timeCountDefault / 16)
            timeCount = timeCountDefault;

         if (TMR1 > timeCount)
         {
             LATFbits.LATF1 = !LATFbits.LATF1;
             TMR1 = 0;
         }
     }
}

void    set_timer()
{
    /*
    ** On selectionne d'abord l'oscillateur externe primaire en tant que source
    ** NOTE :
    **  Cette partie est commentee car j'ai effectue ces operations dans les
    **  configbits
    */
 //   DEVCFG1bits.FNOSC = 0x3; // selectionne le POSC comme oscillateur par
                            // defaut, avec PLL

 //   DEVCFG1bits.POSCMOD = 0x0; // On selectionne le mode External Clock (EC) de
                                // l'oscillateur

 //   OSCCONbits.PBDIV = 0x3; // On regle le postscaler a 8

    /*
    ** On regle ensuite le timer 1 pour utiliser cette source :
    */
    T1CONbits.ON = 0; // On desactive le timer
    T1CONbits.TGATE = 0; // Gated time accumulation desactive
    T1CONbits.TCS = 0; // On choisi comme source le TPBCLK
    T1CONbits.TCKPS = 0x3; // Valeur du prescaler (de 00 = 1:1 a 11 = 1:256)
    TMR1 = 0; // On reset la valeur du registre de comptage pour le timer 1
    T1CONbits.ON = 1; // On active le timer
}


/*
 * QUESTIONS
 *
 * - Qu'es-ce que le PLL ? Pourquoi ne peut-on pas debugger sans PLL ?
 *
*/