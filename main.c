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

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;
     while (1)
     {
         debounce = 0;
         while (!PORTDbits.RD8)
             debounce++;
         if (debounce > 2)
             LATFbits.LATF1 = !LATFbits.LATF1;
     }
     return (1);
}

