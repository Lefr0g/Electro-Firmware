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
    u8  debounce;
    u8  timeCount;

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

    OSCCONbits.COSC = 0x2;
    T1CONbits.TCS = 0;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0x2;
    TMR1 = 0;
    T1CONbits.ON = 1;

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

