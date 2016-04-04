/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on March 29, 2016, 3:27 PM
 */

#include "types.h"
#include <p32xxxx.h>

void    set_timer()
{
    // On selectionne d'abord l'oscillateur externe primaire en tant que source
    
    
    // On regle ensuite le timer 1 pour utiliser cette source :
    T1CONbits.ON = 0; // On desactive le timer
    T1CONbits.TGATE = 0; // Gated time accumulation desactive
    T1CONbits.TCS = 0; // On choisi comme source le TPBCLK
    T1CONbits.TCKPS = 0; // Valeur du prescaler (de 00 = 1:1 a 11 = 1:256)
    TMR1 = 0; // On reset la valeur du registre de comptage pour le timer 1
    T1CONbits.ON = 1; // On active le timer
}

int main()
{
    u8  debounce;
    u8  timeCount;

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

    set_timer();
    
    OSCCONbits.COSC = 0x2;
//    T1CONbits.ON = 1;

     while (1)
     {
         if (TMR1 > 3906)
         {
             LATFbits.LATF1 = !LATFbits.LATF1;
             TMR1 = 0;
         }

         /* PART 1
         debounce = 0;
         while (!PORTDbits.RD8)
             debounce++;
         if (debounce > 2)
             LATFbits.LATF1 = !LATFbits.LATF1;
          */
     }
     return (1);
}

