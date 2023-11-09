/*********************************************************
 * ball_movement.c
 *
 * Implements the state diagram for the ball's movement
 * Only a few states are completed, refer to the CW description
 * to find out about the remaining states
 *
 * Created on: Nov 2017
 * Author: M MATA (GCU)
 *********************************************************/
#include "ball_movement.h"
#include "general_settings.h"
#include "hal_lcd.h"
#include "msp430f5438a.h"



int top_wall_reached()
{
    if(yBall <= BALL_RADIUS) //top wall reached
    {
        yBall = 1+BALL_RADIUS; //do not overwrite the wall
        return 1;
    }
    else return 0;
}

int right_wall_reached()
{
    if(xBall >= LCD_COL-1-BALL_RADIUS) //right wall reached
    {
        xBall = LCD_COL-2-BALL_RADIUS; //do not overwrite right wall/racket
        return 1;
    }
    else return 0; //right wall not reached
}

int left_wall_reached()
{
    if(xBall <= BALL_RADIUS)  //left wall reached
    {
        xBall = 1+BALL_RADIUS; //do not overwrite wall/racket
        return 1;
    }
    else return 0; //right wall not reached
}

int bottom_wall_reached()
{
    if(yBall >= LCD_ROW-1-BALL_RADIUS) //bot wall reached
    {
        yBall = LCD_ROW-2-BALL_RADIUS; //do not overwrite wall
        return 1;
    }
    else return 0; //bottom wall not reached


}


int P1_racket_hit() //check ball vs left racket
{
 if( (yBall <= (yR1 + HALF_RACKET_SIZE)) && (yBall >= (yR1 - HALF_RACKET_SIZE)) )
   {
     P1OUT ^= (BIT0+BIT1); //toggles LEDs to alternate between them

     return 1;
   }

 else
     return 0;
}

int P2_racket_hit() //check ball vs right racket
{
 if( (yBall <= (yR2 + HALF_RACKET_SIZE)) && (yBall >= (yR2 - HALF_RACKET_SIZE)) )
   {
     P1OUT ^= (BIT0+BIT1); //toggles LEDs to alternate between them

     return 1;
   }

 else
     return 0;
}

//Update the state and position of the ball
//(CPU is awaken by TimerA1 interval ints)
void ball_update(void)
{
 //calculate new position and bouncing
 switch(ballState)
 {
  case 0: //"Start" state, init ball position
          yBall = LCD_ROW >> 1;
          xBall = LCD_COL >> 1;
          xBall_old2 = xBall;
          yBall_old2 = yBall;
          xBall_old = xBall;
          yBall_old = yBall;
          //choose next state to start ball movement
          ballState = 1;
          break;
  case 1: //move Right
          xBall = xBall + 2;
          //check right wall bounce
          if(right_wall_reached())
          {

              if(P2_racket_hit())
              {
                  if(R2Dir == UP) //racket moving up apply effect
                  { ballState = 7; }
                  else
                  {
                     if(R2Dir == DOWN) //racket moving down apply effect
                      {ballState = 9;}
                     else
                      {ballState = 8;} //if neither
                  }
              }
              else //goal! P1 scores
              {
                  ballState = 15;


              }


          }

          break;
  case 2: //move Right and a bit Up
          xBall = xBall + 2;
          yBall = yBall - 1;
          //Check top wall bounce
          if(top_wall_reached())
              ballState = 14; //Top wall hit, bounce to 14
          //check right wall bounce
          else if(right_wall_reached())
          {

             if(P2_racket_hit())
             {
                 if(R2Dir == UP) //apply effect if racket up
                 {ballState = 6;}
                 else
                 {

                     if(R2Dir == DOWN) //apply effect if racket down
                     {ballState = 8;}
                     else             //if neither
                     {
                         ballState = 7;

                     }

                 }

             }

             else //Goal!!
             {ballState = 15;}
          }
          break;
  case 3: //move right and up more
           xBall = xBall + 2;
           yBall = yBall - 2;
            //Check top wall bounce
            if(top_wall_reached())
              ballState = 13; //Top wall hit, bounce to 13
              //check right wall bounce
             else if(right_wall_reached())
             {

                if(P2_racket_hit())
                {
                    if(R2Dir == UP) //apply effect if racket UP
                    {ballState = 5;}
                    else
                    {
                      if(R2Dir == DOWN) //apply effect if racket Down
                      { ballState = 7;}
                      else
                      {ballState = 6;} //if neither

                    }
                }
                else //goal!
                {
                    ballState = 15;

                }


             }

          break;
  case 4: //move right and end up just less than 90deg
          xBall = xBall + 1;
          yBall = yBall - 2;
          if(top_wall_reached())
            ballState = 12; //Top wall hit, bounce to 12
            //check right wall bounce
          else if(right_wall_reached())
          {

              if(P2_racket_hit())
                            {
                                if(R2Dir == UP) //apply effect if racket going up
                                {ballState = 5;}
                                else
                                {
                                  if(R2Dir == DOWN) //apply effect if racket going down
                                  { ballState = 6;}
                                  else
                                  {ballState = 5;} //if neither

                                }
                            }
                            else //goal!
                            {
                                ballState = 15;

                            }

          }



          break;
  case 5: //move left and end up just less than 90deg
          xBall = xBall -1;
          yBall = yBall -2;
          //check top wall bounce
          if(top_wall_reached())
             ballState = 11; //top wall hit, bounce to 11
                //check left wall for racket hit or goal
                else if(left_wall_reached())
                {

                    if(P1_racket_hit())
                    {
                     if(R1Dir == UP) //apply effect if racket up
                     { ballState = 4;}
                      else
                      {
                       if(R1Dir == DOWN) //apply effect if racket down
                       {ballState = 3;}
                       else
                       { ballState =4;} //if neither
                      }
                    }

                    else //Goal occurs
                    {
                        ballState = 16;

                    }

                }

          break;
  case 6: //move left and up a bit more
          xBall = xBall -2;
          yBall = yBall -2;
          //check top wall bounce
          if(top_wall_reached())
             ballState = 11; //top wall hit, bounce to 11
          //check left wall for racket hit or goal
                else if(left_wall_reached()) //left wall hit then look for following cases
                {

                    if(P1_racket_hit())
                    {
                       if(R1Dir == UP) //racket moving up apply effect on ball
                       { ballState =4;}
                       else
                       {
                           if(R1Dir == DOWN) //racket moving down, apply effect
                           { ballState = 2;}
                           else
                           { ballState = 3;} //if neither
                       }

                   }

                    else //Goal!
                    {
                        ballState = 15;

                    }

                }

          break;
  case 7: //move left and up a bit
          xBall = xBall -2;
          yBall = yBall -1;
          //check top wall bounce
          if(top_wall_reached())
              ballState = 9; //top wall hit, bounce to 9
          //check left wall for racket hit or goal
          else if(left_wall_reached())
          {
              //If racket is here bounce, otherwise it's a goal
                        if(P1_racket_hit())
                        {
                           if(R1Dir == UP) //up effect on ball
                           { ballState = 3; }
                           else
                           {
                               if(R1Dir == DOWN) //down effect on ball
                                {   ballState = 1; }
                               else //no effect, normal bounce
                                {   ballState = 2; }
                           }
                        }
                        else //Goal! Player 1 missed the ball
                         { ballState = 16; }
          }

          break;
  case 8: //move Left
          xBall = xBall - 2;
          //check left wall reached
          if(left_wall_reached())
          {

            //If racket is here bounce, otherwise it's a goal
            if(P1_racket_hit())
            {
               if(R1Dir == UP) //up effect on ball
               { ballState = 2; }
               else
               {
                   if(R1Dir == DOWN) //down effect on ball
                    {   ballState = 14; }
                   else //no effect, normal bounce
                    {   ballState = 1; }
               }
            }
            else //Goal! Player 1 missed the ball
             {
                ballState = 16;

             }
          }
          break;
  case 9: //move
          xBall = xBall -2;
          yBall = yBall +1;
                //check top wall bounce
                if(bottom_wall_reached())
                    ballState = 7; //top wall hit, bounce to 9
                //check left wall for racket hit or goal
                else if(left_wall_reached())
                {

                    if(P1_racket_hit())
                    {
                      if(R1Dir == UP)
                      {ballState = 1;} //apply effect if going up
                      else
                      {
                         if(R1Dir == DOWN)
                         { ballState = 13;} //apply effect if going down
                         else
                         { ballState = 14;}  //if neither
                      }
                    }
                    else //goal
                    {
                        ballState = 16;

                    }
                }

          break;
  case 10://move left and down
           xBall = xBall - 2;
           yBall = yBall + 2;
                     //check bottom wall
                     if(bottom_wall_reached())
                        ballState = 6 ; //bottom wall hit, bounce to 11
                     //check right wall bounce
                     else if(left_wall_reached())
                            {

                               if(P1_racket_hit()) //if racket is present go to bounce otherwise its a goal
                                 {
                                     if(R1Dir == UP)  //apply UP effect on ball
                                       { ballState = 14; }

                                      else
                                       {
                                         if(R1Dir == DOWN) //apply down effect on ball
                                           {  ballState = 12; }

                                         else
                                            { ballState = 13;} //if neither occur


                                       }
                                  }
                                           else //GOAL! Player missed the ball
                                           {
                                               ballState = 16;

                                           }
                              }


          break;
  case 11://move
           xBall = xBall - 1;
           yBall = yBall + 2;
               //check bottom wall
               if(bottom_wall_reached())
                  ballState = 5 ; //bottom wall hit, bounce to 11
               //check right wall bounce
               else if(left_wall_reached())
                      {

                         if(P1_racket_hit()) //if racket is present go to bounce otherwise its a goal
                           {
                               if(R1Dir == UP)  //apply UP effect on ball
                                 { ballState = 13; }

                                else
                                 {
                                   if(R1Dir == DOWN) //apply down effect on ball
                                     {  ballState = 12; }

                                   else
                                      { ballState = 12;} //if neither occur


                                 }
                            }
                                     else //GOAL! Player missed the ball
                                     {
                                         ballState = 16;

                                     }
                        }


          break;
  case 12://move
          xBall = xBall + 2;
          yBall = yBall + 2;
          //check bottom wall
          if(bottom_wall_reached())
             ballState = 4 ; //bottom wall hit, bounce to 11
          //check right wall bounce
          else if(right_wall_reached())
                 {

                      if(P2_racket_hit()) //if racket is present go to bounce otherwise its a goal
                        {
                           if(R2Dir == UP)  //if racket move UP, apply effect on ball
                            { ballState = 10; }

                               else
                               {
                                 if(R2Dir == DOWN) //apply effect on ball if racket move down
                                   {  ballState = 11; }

                                   else            //if neither occur
                                   { ballState = 11;}


                               }

                         }
                                else //GOAL! Player missed the ball
                                {
                                    ballState = 15;

                                }
                   }



          break;
  case 13://move down and right more
          xBall = xBall + 2;
          yBall = yBall + 2;
          //check bottom wall
          if(bottom_wall_reached())
              ballState = 3; //bottom wall hit, bounce to 10
          //check right wall bounce
          else if(right_wall_reached())
                 {

                       if(P2_racket_hit()) //if racket is present go to bounce otherwise its a goal
                       {
                        if(R2Dir == UP)  //apply UP effect on ball
                        { ballState = 9; }


                       else
                        {
                           if(R2Dir == DOWN) //apply down effect on ball
                           {  ballState = 11; }

                           else             // if neither
                           { ballState = 10;}


                        }
                       }
                        else //GOAL! Player missed the ball
                        {
                           ballState = 15;

                        }
                   }


          break;
  case 14://move down and right a bit
          xBall = xBall + 2;
          yBall = yBall + 1;
          //Check bottom wall bounce
          if(bottom_wall_reached())
            ballState = 2; //bottom wall hit, bounce to 2
                //check right wall bounce
          else if(right_wall_reached())
          {

              if(P2_racket_hit()) //if racket is present go to bounce otherwise its a goal
              {
               if(R2Dir == UP) //if racket is going up apply effect
               { ballState = 8; }


              else
               {
                  if(R2Dir == DOWN) //if going down apply effect
                  {  ballState = 10; }

                  else              //if neither
                  { ballState = 9;}


               }
              }
               else //GOAL!
               {
                  ballState = 15;

               }
          }
          break;
  case 15://Right-hand player missed the ball!
          //A very simplistic game end handling
               halLcdClearScreen(); //CLEAR SCREEN
               halLcdPrintLine("GOAL PLAYER 2", 4, OVERWRITE_TEXT);//PRINT MESSAGE
               halLcdPrintLine(" Reset to start", 6, OVERWRITE_TEXT);//PRINT MESSAGE
               //stop TimerA1. This prevents new LCD and ball updates
               //but user input is operational because is driven by TimerB0
               TA1CTL= TA1CTL & ~(BIT5 + BIT4); //MC=00 (bits 5,4) 0b11001111

          break;
  case 16://Left-hand player missed the ball!
          //A very simplistic game end handling
          halLcdClearScreen(); //CLEAR SCREEN
          halLcdPrintLine("GOAL PLAYER 1", 4, OVERWRITE_TEXT);//PRINT MESSAGE
          halLcdPrintLine(" Reset to start", 6, OVERWRITE_TEXT);//PRINT MESSAGE
          //stop TimerA1. This prevents new LCD and ball updates
          //but user input is operational because is driven by TimerB0
          TA1CTL= TA1CTL & ~(BIT5 + BIT4); //MC=00 (bits 5,4) 0b11001111
          break;
 }
}
