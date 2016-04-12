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
void __ISR(_EXTERNAL_1_VECTOR, IPL3) btn_ex3 (void)
{
        PR1 /= 2;
        if (PR1 < 3906 / 16)
            PR1 = 3906;
        IFS0bits.INT1IF = 0;
        TMR1 = 0;
}
*/

/*exo4*/
u8  dim_flag = 0;
u8  dim_way = 0;
u16 timer1_stamp = 0;
u16 timer1_count = 0;
u16 timer1_blink_period = 3906;
u16 timer1_dim_period = 100;
u16 timer1_default_period = 62500;

u16 timer2_count = 0;
u16 timer2_stamp = 0;
u16 timer2_period_default = 62500;

void __ISR(_EXTERNAL_1_VECTOR, IPL3) btn_ex4 (void)
{
    u8  debounce = 0;

    while (debounce < 5)
        debounce++;

    if (!INTCONbits.INT1EP)
    {
        timer2_stamp = TMR2;
        INTCONbits.INT1EP = 1;
        PR2 = 0;
    }
    else if (INTCONbits.INT1EP && TMR2 < timer2_period_default * 2
            && !timer2_count)
    {
        dim_flag = 0;
        PR1 /= 2;
        if (PR1 < timer1_blink_period / 16)
            PR1 = timer1_blink_period;
    }
    else
    {
        dim_flag = 1;
    // Switch to dim
    }
    IFS0bits.INT1IF = 0;
    TMR1 = 0;
}

void __ISR(_TIMER_1_VECTOR, IPL4) timer1_ex4 (void)
{
    LATFbits.LATF1 = !LATFbits.LATF1;
    IFS0bits.T1IF = 0;
}

void __ISR(_TIMER_2_VECTOR, IPL5) timer2_ex4 (void)
{
    if (!dim_flag)
        timer2_count++;
    else
    {
        if (dim_way)
            PR2++;
        else
            PR2--;
        if (PR2 >= PR1)
            dim_way = !dim_way;
        LATFbits.LATF1 = !LATFbits.LATF1;
    }
    IFS0bits.T2IF = 0;
}

/*
void __ISR(_TIMER_1_VECTOR, IPL4) blink_ex3 (void)
{
    LATFbits.LATF1 = !LATFbits.LATF1;
    IFS0bits.T1IF = 0;
}
*/

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
    IPC1bits.T1IP = 4; // priorite Timer1 interrupt
    IEC0bits.T2IE = 1; // enable Timer2 Interrupt
    IPC2bits.T2IP = 5; // priorite Timer2 interrupt

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

    T2CONbits.ON = 0; // On desactive le timer
    T2CONbits.TGATE = 0; // Gated time accumulation desactive
//    T2CONbits.TCS = 0; // On choisi comme source le TPBCLK
    T2CONbits.TCKPS = 0x7; // Valeur du prescaler (de 00 = 1:1 a 11 = 1:256)
    TMR2 = 0; // On reset la valeur du registre de comptage pour le timer 1
    T2CONbits.ON = 1; // On active le timer
}


/*
 * QUESTIONS
 *
 * - Qu'es-ce que le PLL ? Pourquoi ne peut-on pas debugger sans PLL ?
 *
*/