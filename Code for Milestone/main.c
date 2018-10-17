#include <msp430.h> 


/**
 * main.c
 * Authored by: DJ Stahlberger and Christopher Satriale
 */

volatile unsigned char total; //to compare count to the number of bytes sent to the board
volatile unsigned char count; //to incrementally use and send certain data based off the value

void PinSetup(void){

    //P1.6 -- Red -- TA0CCR0
    P1SEL |= BIT6;
    P1SEL &= ~BIT6;             //Connects P1.6 to TimerA
    P1DIR |= BIT6;              //Sets output direction

    //P2.1 -- Green -- TA1CCR1
    P2SEL |= BIT1;
    P2SEL &= ~BIT1;             //Connects P2.1 to TimerA
    P2DIR |= BIT1;              //Sets output direction

    //P2.4 -- Blue -- TA1CCR2
    P2SEL |= BIT4;
    P2SEL &= ~BIT4;             //Connects P2.4 to TimerA
    P2DIR |= BIT4;              //Sets output direction
}

void timerASetup(void){

    //Timer 0 configuration

    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;        // SMCLK, up mode, /4, clear

    TA0CCR0 = 255;                                  // Sets the PWM Period
    TA0CCR1 = 0;                                    // CCR1 duty cycle
    TA0CCTL1 = OUTMOD_3;                            // CCR1 set reset


    //Timer 1 configuration

    TA1CTL = TASSEL_2 + MC_1 + ID_2 + TACLR;        // SMCLK, up mode, /4, clear

    TA1CCR0 = 255;                                  // Sets the PWM Period
    TA1CCR1 = 0;                                    // CCR1 PWM duty cycle
    TA1CCR2 = 0;                                    // CCR2 PWM duty cycle

    TA1CCTL1 = OUTMOD_3;                            // CCR1 set reset
    TA1CCTL2 = OUTMOD_3;                            // CCR2 set reset
}

void UARTsetup(void){

    P1SEL |= BIT1 + BIT2;                        //sets P1.1 to RX and P1.2 to TX
    P1SEL2 |= BIT1 + BIT2;                       //sets P1.1 to RX and P1.2 to TX

    UCA0CTL1 |= UCSSEL_2;                        //sets BR clock to SMCLCK
    UCA0BR0 = 109;                               //sets Baud rate to 9600
    UCA0BR1 = 0;                                 //sets Baud rate to 9600
    UCA0MCTL = UCBRS_2;

    UCA0CTL1 &= ~UCSWRST;                        //Disables UART reset
    UC0IE |= UCA0RXIE;                           //Enables UART interrupt
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;                    //stop watch dog timer

    PinSetup();
    timerASetup();
    UARTsetup();

    __bis_SR_register(LPM0_bits + GIE);         //global interrupts enabled + low power mode
}



//TA0CCR0 = RED
//TA1CCR1 = GREEN
//TA1CCR2 = BLUE
//UCA0RXBUF DATA RECIEVED
//UCA0TXBUF DATA TRANSMITTED
#pragma vector = USCIABORX_VECTOR;              //Setting the interrupt condition for the button press
__interrupt void usciaborx (void){

    char data = UCA0RXBUF;                      //stores byte from UART into data char

    if(count == 0){                             //Initial if statement designed to run at the first package byte
        count = data;                           //count gets the number of bytes in the package

        total = data;                           //total also gets number of bytes in the package


        if(data >=8 | (data-2)%3 == 0)          //More than 8 bytes left or there are enough
                                                //bytes for another node to use
            UCA0TXBUF = data - 3;               //package size gets sent to the next node
    }
    else if(total - count = 1)                  //if the byte being read is directly after the size byte
        TA0CCR0 = data;                         //set the value for the red
    else if(total - count = 2)                  //if the byte being read is 2 after the size byte
        TA1CCR1 = data;                         //set the value for the green
    else if(total - count = 3)                  //if the byte being read is 3 after the size byte
        TA1CCR2 = data;                         //sets the value for the blue
    else{
        if(data >=8 | (data-2)%3 == 0)          //more than 8 bytes left or there are enough
                                                //bytes for another node to use
            UCA0TXBUF = data;                   //transmit the byte to the next node
    }

    count--;                                    //incrementally counts down from the total package size until 0

   }
















