//******************************************************************************
//  Pong.c
//
//  Description: Starting point for the Pong game coursework
//  Core functionality is completed (timed operation)
//  ACLK = REFO = LFXT1 32kHz, MCLK = SMCLK = 1MHz
//
//  M. MATA
//  GCU
//  November 2017
//******************************************************************************	
#include    "msp430f5438a.h"
#include	"hal_UCS.h"
#include 	"hal_PMM.h"
#include    "hal_lcd.h"
#include    "general_settings.h"
#include    "ball_movement.h"

/* 5xx functions / variables */ 
void halBoardInit(void);
void halADCInit(void);
void LCDInit(void);
void TimerB0Init(void);
void TimerA1Init(void);
void GameStartInit(void);
void UserInputs_update(void);
void LCD_update(void);

volatile unsigned int LCD_intervals = 0; //count number of base intervals elapsed
volatile unsigned int Ball_intervals = 0; //count number of base intervals elapsed
//volatile =  compiler will not optimize these variables

//This is kept as an ADC12 example, and allows battery monitoring
volatile unsigned long temp_vcc;
void format_voltage_string(unsigned int v);
char LCD_string[5]="x.xV\0"; //test only

//main function
void main(void)
{  
  volatile unsigned long  batt_voltage; 
  
  WDTCTL = WDTPW+WDTHOLD; // Stop WDT
  
  // Initialize board
  halBoardInit();  

  /* Initialize ADC12 & REF module to sample battery voltage */
  halADCInit();

  /* Initialize TimerB0, it drives ADC12 and times input updates */
  TimerB0Init();

  /* Initialize TimerA1, it times ball and LCD updates */
  TimerA1Init();

  /* Initialize LFXT1, it will drive TA1 and TB0 */
  P7SEL |= BIT1+BIT0;    // Enable crystal pins
  LFXT_Start(XT1DRIVE_0);        

  //Initialize LCD and backlight
  LCDInit();

  //Initialize game variables
  GameStartInit();

  while(1) //infinite main loop
  {
    // Bit-set (LPM3_bits + GIE) in SR register to enter LPM3 mode    
    __bis_SR_register(LPM3_bits + GIE); 
    __no_operation(); //for debug

    //CPU CONTINUES HERE WHEN IT IS AWAKEN AT THE END OF ADC12 or TimerA1 ISR

     // Calculate voltage to test ADC functionality
     // 10 * Batt voltage = 2.0V(A11/4096) * 2 * 10 = 40(A11)/4096
     batt_voltage = (temp_vcc * 40) >> 12;
     format_voltage_string(batt_voltage);

    if(InputUpdatePending)
    {
     InputUpdatePending=0;
     UserInputs_update();
    }
    if(BallUpdatePending)
    {
     BallUpdatePending=0;
     ball_update();
    }
    if(LCDUpdatePending)
    {
     LCDUpdatePending=0;
     LCD_update();
    }

  }   	  
}

//LCD initialization
void LCDInit(void)
{
 halLcdInit();
 halLcdBackLightInit();
 halLcdSetBackLight(LCD_BACKLIGHT_LEVEL);
 halLcdSetContrast(LCD_CONTRAST_LEVEL);
 halLcdClearScreen();
}

//This function can be used to initialize game variables at boot-up
//Most variables are  declared into general_settings.h
void GameStartInit()
{
 InputUpdatePending = 0;
 BallUpdatePending = 0;
 LCDUpdatePending = 0;
 ballState = 0; //initial ball state

 //Draw top, right and bottom walls
 halLcdLine(0,0, LCD_COL-1,0, PIXEL_ON);
 halLcdLine(LCD_COL-1,0, LCD_COL-1,LCD_ROW-1, PIXEL_ON);
 halLcdLine(0,LCD_ROW-1, LCD_COL-1,LCD_ROW-1, PIXEL_ON);

 //Initial position of racket 1
 xR1 = 0; //left-hand side
 yR1 = LCD_ROW >> 1; //middle row
 xR1_old = xR1;
 yR1_old = yR1;

 //intial position of racket 2
 xR2 = 130; //right hand side
 yR2 = LCD_ROW >> 1; //middle row
 xR2_old = xR2;
 yR2_old = yR2;

}

//Read user inputs here (CPU is awaken by ADC12 conversion)
void UserInputs_update(void)
{
 R1Dir = STOP;
 //EXAMPLE: read button SW1
 if(!(P2IN & BIT6)) //SW1 pressed
   {
     if (yR1 > HALF_RACKET_SIZE) //avoid overwriting top wall
     {
      yR1--;
     }//move racket 1 pixel up shorthand calculation(check whiteboard for full calc)
     R1Dir = UP;
   }


//first task here move down + int pin 7 using sw2
 if(!(P2IN & BIT7)) //SW2 pressed by user
  {
     if(yR1 + HALF_RACKET_SIZE < LCD_ROW - 1)//avoid overwriting bottom wall
     {
         yR1++; //move racket 1 pixel down
     }

   R1Dir = DOWN; //take note of what the direction was
  }
 R2Dir = STOP;

 if(!(P2IN & BIT4)) //SW1 pressed       //copy of previous setup to allow for use of another racket
   {
     if(yR2 > HALF_RACKET_SIZE)
     {
         yR2--; //move racket 1 pixel up
     }
     R2Dir = UP; //note direction
   }

 if(!(P2IN & BIT5))
   {
     if(yR2 + HALF_RACKET_SIZE < LCD_ROW - 1) //avoid overwriting bottom wall
     {
         yR2++; //move racket 1 pixel down
     }
     R2Dir = DOWN; //note direction
   }


}
//Update drawings in LCD screen (CPU is awaken by TimerA1 interval ints)
void LCD_update(void)
{
 //update older positions to clear old racket and draw new one
 //clear old racket1
 halLcdVLine(xR1_old, yR1_old - HALF_RACKET_SIZE, yR1_old + HALF_RACKET_SIZE, PIXEL_OFF);
 halLcdVLine(xR1_old + 1, yR1_old - HALF_RACKET_SIZE, yR1_old + HALF_RACKET_SIZE, PIXEL_OFF);
 //Draw new racket1
 halLcdVLine(xR1, yR1 - HALF_RACKET_SIZE, yR1 + HALF_RACKET_SIZE, PIXEL_ON);
 halLcdVLine(xR1 + 1, yR1 - HALF_RACKET_SIZE, yR1 + HALF_RACKET_SIZE, PIXEL_ON);
 yR1_old = yR1;
 xR1_old = xR1;

 //clear old racket2
  halLcdVLine(xR2_old, yR2_old - HALF_RACKET_SIZE, yR2_old + HALF_RACKET_SIZE, PIXEL_OFF);
  halLcdVLine(xR2_old + 1, yR2_old - HALF_RACKET_SIZE, yR2_old + HALF_RACKET_SIZE, PIXEL_OFF);
  //Draw new racket2
  halLcdVLine(xR2, yR2 - HALF_RACKET_SIZE, yR2 + HALF_RACKET_SIZE, PIXEL_ON);
  halLcdVLine(xR2 + 1, yR2 - HALF_RACKET_SIZE, yR2 + HALF_RACKET_SIZE, PIXEL_ON);
  yR2_old = yR2;
  xR2_old = xR2;

 //Clear oldest ball
 halLcdCircle(xBall_old2, yBall_old2, BALL_RADIUS, PIXEL_OFF);
 halLcdPixel(xBall_old2, yBall_old2, PIXEL_OFF);
 //Draw ball trail
 halLcdCircle(xBall_old, yBall_old, BALL_RADIUS, PIXEL_ON);
 halLcdPixel(xBall_old, yBall_old, PIXEL_ON);
 //Draw new ball
 halLcdCircle(xBall, yBall, BALL_RADIUS, PIXEL_ON);
 halLcdPixel(xBall, yBall, PIXEL_ON);
 //update older positions for drawing ball trail and deleting old ball
 xBall_old2=xBall_old;
 yBall_old2=yBall_old;
 xBall_old=xBall;
 yBall_old=yBall;

}

// create string with voltage measurement
void format_voltage_string(unsigned int voltage)
{
  unsigned char ones = '0';
  unsigned char tenths = '0';
  while(voltage >= 10)
  {
    ones++;
    voltage -= 10;
  }
  tenths += voltage;
  LCD_string[0] = ones;
  LCD_string[2] = tenths;
}

/*********************************************************
 ********** System initializations ***********************
 *********************************************************/
//MSP430 pins initialization
void halBoardInit(void)
{
  // Tie unused ports and pins as LOW outputs to save power
    //PA = P1 and P2 (16 bits)
    //PB = P3 and P4 (16 bits)
    //PC = P5 and P6 (16 bits)
    //PD = P7 and P8 (16 bits)
    //PE = P9 and P10 (16 bits)
  PAOUT  = 0; PADIR  = 0xFFFF; PASEL  = 0;
  PBOUT  = 0; PBDIR  = 0xFFFF; PBSEL  = 0;
  PCOUT  = 0; PCDIR  = 0xFFFF; PCSEL  = 0;
  PDOUT  = 0; PDDIR  = 0xFFFF; PDSEL  = 0;
  // P10.0 to USB RST pin, if enabled with J5
  PEOUT  = 0; PEDIR  = 0xFEFF; PESEL  = 0;
  P11OUT = 0; P11DIR = 0xFF;   P11SEL = 0;
  PJOUT  = 0; PJDIR  = 0xFF;

  P6OUT = 0x40;      // Shut down audio output amp
  P5DIR &= ~0x80;    // USB RX Pin, Input with pull-down
  P5OUT &= ~0x80;
  P5REN |= 0x80;

  //Now configure SW1 (P2.6) as input with pull-down (example)
  P2DIR &= ~BIT6; //pin 6 input
  P2REN = P2REN | BIT6; //pin 6 internal pull R enabled
  P2OUT = P2OUT | BIT6; //pin 6 pull-down

  P2DIR &= ~BIT7; //pin 7 input                         /*
  P2REN = P2REN | BIT7; //pin 7 internal pull R enabled   int SW2 (P2.7) as input
  P2OUT = P2OUT | BIT7; //pin 7 pull-down               */

  //configure SW2 using pins 4 and 5
    P2DIR &= ~(BIT4 + BIT5);
    P2REN = P2REN | (BIT4 + BIT5);
    P2OUT = P2OUT | (BIT4 + BIT5);

    //now configure LEDS (P1.0 and P1.1)
    P1DIR &= ~(BIT0+BIT1);
    P1REN = P1REN | (BIT0+BIT1);
    P1OUT = P1OUT | BIT1;
    P1OUT = P1OUT &~ BIT0;

}

//Inits ADC12 to read battery voltage triggered from TB0.OUT3
void halADCInit(void)
{
  // 5.4MHz (MODOSC_max) * 30 us (t_sensor)= 162 cycles min sampling time
  ADC12CTL0 = ADC12SHT0_7+ADC12ON+ADC12MSC;     // Set sample time to 192 cycles
  // Enable TB0.OUT1 as ADC trigger (ADC12SHS_3), enable sequence-of-channels
  ADC12CTL1 = ADC12SHS_3 + ADC12CONSEQ_1+ADC12SHP; // ... also enable sample timer

  //define conversion sequence (just Batt voltage so only ADC12MEM0 used)
  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_11 + ADC12EOS;// ADC input ch A11 = Vcc
  ADC12IE |= BIT0;                             //Enable ADC12MEM0IFG interrupt

  /* Initialize the shared reference module */
  REFCTL0 |= REFMSTR + REFVSEL_1 + REFON;      // Configure internal 2.0V reference
  ADC12CTL0 |= ADC12ENC;                // Enable conversions to configure REF
}

//NOTE: TimerA0 is initialized inside hal_lcd.c because it's already used for the LCD backlight PWM
//Initialize TimerA1 as basic intervals generator
void TimerA1Init(void)
{
  //TA1CCR0   = TIMING_BASE_mS*(Faclk/1000);
  // TIMING_BASE_mS*(32.768) ~ TIMING_BASE_mS*(33)
  TA1CCR0   = TIMING_BASE_mS*33;
  TA1CCTL0 = CCIE; //Enable TA1CCR0 CCIFG int
  TA1CTL    = TASSEL_1 + MC_1 + TACLR; // ACLK, UP mode. Do not enable TAIE (rollover int)
}

//Initialize TimerB0 to drive the ADC12 and inputs reading
void TimerB0Init(void)
{
  //TB0CCR0   = INPUT_INTERVAL_mS*(Faclk/1000);
  // INPUT_INTERVAL_mS*(32.768) ~ INPUT_INTERVAL_mS*(33)
  TB0CCR0   = INPUT_INTERVAL_mS*33; //set timer interval
  // Provide CCR1 value, time until CCR1 match must be greater than 75 us!
  TB0CCR1   = 3;         //this time will be 3 * 1/32768kHZ (> 75usec)
  TB0CCTL1  = OUTMOD_3;  // CCR1 output mode => SET/RESET. Do not enable TB0CCR3 INT
  // ACLK, UP mode, clear TBR, enable rollover INT
  TB0CTL    = TBSSEL_1 + MC_1 + TBCLR + TBIE;
}

/***********************************************************************
 *************** Interrupt Service Routines (ISRs) *********************
 ***********************************************************************/
// Timer A1 CCR0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
 //interrupt flag CCIFG is automatically cleared when servicing this ISR
  if(!LCDUpdatePending) //if update still pending, skip this interval
 {
   LCD_intervals++;
   if(LCD_intervals>=(LCD_INTERVAL_mS/TIMING_BASE_mS)) //time to run the ball update
   {
      LCDUpdatePending = 1; //warn the CPU that LCD update is required
      LCD_intervals = 0;
   }
 }

 if(!BallUpdatePending) //if update still pending, skip this interval
 {
   Ball_intervals++;
   if(Ball_intervals>=(BALL_INTERVAL_mS/TIMING_BASE_mS)) //time to run the ball update
   {
     BallUpdatePending = 1; //warn the CPU that ball update is required
     Ball_intervals = 0;
   }
 }

 if(BallUpdatePending || LCDUpdatePending)
 { //keep CPU active after ISR to process updates in main loop
   __bic_SR_register_on_exit(LPM3_bits);
 }
}

// Timer B0 overflow and CCRx (x>0) interrupt service routine
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
  switch(__even_in_range(TB0IV,14))
  {
    case  0: break;                // No interrupt
    case  2: break;                // TB0CCR1 not used
    case  4: break;                // TB0CCR2 not used
    case  6: break;                // TB0CCR3 not used
    case  8: break;                // TB0CCR4 not used
    case 10: break;                // TB0CCR5 not used
    case 12: break;                // TB0CCR6 not used
    case 14:                       // TBIFG overflow, enable new ADC
            REFCTL0 |= REFON;      // Enable internal reference
            ADC12CTL0 |= ADC12ENC; // Enable conversions to configure REF
            break;
  }
}

//ADC12 interrupt service routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                  // Vector  0:  No interrupt
  case  2: break;                  // Vector  2:  ADC overflow
  case  4: break;                  // Vector  4:  ADC timing overflow
  case  6:                         // Vector  6:  ADC12IFG0, last conversion now
      //Data is ready (both MEM0 converted)
      ADC12CTL0 &= ~ADC12ENC;  // Disable conversions to disable VREF
      REFCTL0 &= ~REFON;       // Disable internal reference
      temp_vcc = ADC12MEM0;    //Get Vcc digitalisation
      InputUpdatePending = 1; //warn the CPU that input update is required
      //Keep CPU active on exit to process user inputs in main loop
      __bic_SR_register_on_exit(LPM3_bits);
      break;
  case  8:                         // Vector  8:  ADC12IFG1, not in use now
           break;
  case 10: break;                  // Vector 10:  ADC12IFG2
  case 12: break;                  // Vector 12:  ADC12IFG3
  case 14: break;                  // Vector 14:  ADC12IFG4
  case 16: break;                  // Vector 16:  ADC12IFG5
  case 18: break;                  // Vector 18:  ADC12IFG6
  case 20: break;                  // Vector 20:  ADC12IFG7
  case 22: break;                  // Vector 22:  ADC12IFG8
  case 24: break;                  // Vector 24:  ADC12IFG9
  case 26: break;                  // Vector 26:  ADC12IFG10
  case 28: break;                  // Vector 28:  ADC12IFG11
  case 30: break;                  // Vector 30:  ADC12IFG12
  case 32: break;                  // Vector 32:  ADC12IFG13
  case 34: break;                  // Vector 34:  ADC12IFG14
  }
}
