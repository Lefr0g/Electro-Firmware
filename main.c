/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on March 29, 2016, 3:27 PM
 */

#include "types.h"
#include <p32xxxx.h>
#include <sys/attribs.h>

/* exo3
void __ISR(_EXTERNAL_1_VECTOR, IPL3) btn (void)
{
        INTCONbits.INT1EP = 1 //front montant
        PR1 /= 2;
        if (PR1 < 3906 / 16)
            PR1 = 3906;
        IFS0bits.INT1IF = 0;
        TMR1 = 0;
}
*/

/*exo4*/
void    dim(void)
{
    TMR1 = 0;
}

void __ISR(_EXTERNAL_1_VECTOR, IPL3) btn (void)
{
    u8 start;
    u8 end;

    start = 0;      // time stamp 1
    INTCONbits.INT1EP = 1; //front montant
    end = 0;        // time stamp 2
    INTCONbits.INT1EP = 0; //front descendant
    if ((end - start) > 2)
        dim();
    PR1 /= 2;
    if (PR1 < 3906 / 16)
        PR1 = 3906;
    IFS0bits.INT1IF = 0;
    TMR1 = 0;
}

void __ISR(_TIMER_1_VECTOR, IPL4) blink (void)
{
    LATFbits.LATF1 = !LATFbits.LATF1;
    IFS0bits.T1IF = 0;
}

int main()
{

    set_timer();
//  exo1();
//    exo2();
//  exo3();
  exo4();
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

void    exo3()
{
    u8  debounce;

  //  timeCountDefault = 3906;

    PR1 = 3906;

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

    INTCONbits.MVEC = 1; // multi vector mode

    INTCONbits.INT1EP = 0; // front descendant

    IEC0bits.INT1IE = 1; // enable button Interrupt
    IPC1bits.INT1IP = 3; // priorite button interrupt
    IEC0bits.T1IE = 1; // enable Timer1 Interrupt
    IPC1bits.T1IP = 4; // priorite button interrupt

    __builtin_enable_interrupts(); // Tell CPU to look at interrupts

    while (1)
     {
        WDTCONbits.WDTCLR = 1;
     }
}

void    exo4()
{
    u8  debounce;

  //  timeCountDefault = 3906;

    PR1 = 3906;

    LATFbits.LATF1 = 1;
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD8 = 1;

    INTCONbits.MVEC = 1; // multi vector mode

    INTCONbits.INT1EP = 0; // front descendant

    IEC0bits.INT1IE = 1; // enable button Interrupt
    IPC1bits.INT1IP = 3; // priorite button interrupt
    IEC0bits.T1IE = 1; // enable Timer1 Interrupt
    IPC1bits.T1IP = 4; // priorite button interrupt

    __builtin_enable_interrupts(); // Tell CPU to look at interrupts

    while (1)
     {
        WDTCONbits.WDTCLR = 1;
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

 //  OSCCONbits.PBDIV = 0x3; // On regle le postscaler a 8

    /*
    ** On regle ensuite le timer 1 pour utiliser cette source :
    */
    T1CONbits.ON = 0; // On desactive le timer
    T1CONbits.TGATE = 0; // Gated time accumulation desactive
    T1CONbits.TCS = 0; // On choisi comme source le TPBCLK
    T1CONbits.TCKPS = 0x7; // Valeur du prescaler (de 00 = 1:1 a 11 = 1:256)
    TMR1 = 0; // On reset la valeur du registre de comptage pour le timer 1
    T1CONbits.ON = 1; // On active le timer
}


/*
 * QUESTIONS
 *
 * - Qu'es-ce que le PLL ? Pourquoi ne peut-on pas debugger sans PLL ?
 *
*/