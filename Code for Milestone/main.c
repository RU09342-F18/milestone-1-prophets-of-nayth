#include <msp430.h> 


/**
 * main.c
 * Authored by: DJ Stahlberger and Christopher Satriale
 */

void PinSetup(void){

    //p1.6 setup
    P1SEL |= BIT6;
    P1SEL &= ~BIT6;
    P1DIR |= BIT6;

    //P2.1
    P2SEL |= BIT1;
    P2SEL &= ~BIT1;
    P2DIR |= BIT1;

    //P2.4
    P2SEL |= BIT4;
    P2SEL &= ~BIT4;
    P2DIR |= BIT4;
}

void timerASetup(void){

    //Timer 0 configuration

    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;        // SMCLK, up mode, /4, clear
    TA0CCR0 = 255;                                  // Sets the PWM Period
    TA0CCR1 = 0;                                    // CCR1 duty cycle
    TA0CCTL1 = OUTMOD_3;                            //CCR1 set reset


    //Timer 1 configuration

    TA1CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;        // SMCLK, up mode, /4, clear
    TA1CCR0 = 255;                                  // Sets the PWM Period
    TA1CCTL1 = OUTMOD_3;                            // CCR1 set reset
    TA1CCTL2 = OUTMOD_3;                            // CCR2 set reset
    TA1CCR1 = 0;                                    // CCR1 PWM duty cycle
    TA1CCR2 = 0;                                    //CCR2 PWM duty cycle
}

void UARTsetup(void){

    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_2;
    UCA0CTL1 &= ~UCSWRST;
    UC0IE |= UCA0RXIE;
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer

    PinSetup();
    timerASetup();

    _BIS_SR(LPM0_bits + GIE);   //global interrupts enabled
}





#pragma vector = USCIABORX_VECTOR;              //Setting the interrupt condition for the button press
__interrupt void usciaborx (void){


   }
