`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.10.2023 10:35:48
// Design Name: 
// Module Name: Counter_wrapper
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Counter_wrapper(

   input        CLK,
   input        RESET,
   input        ENABLE_IN,
   input        DIRECT,        //input that determines whether the clock count up or down
   output  [3:0] SEG_SELECT,
   output [7:0] DEC_OUT
    );
    
    //wires for all the trigger outs of the different counters
    wire Bit17TriggOut;
    wire Bit4TriggOut0;
    wire Bit4TriggOut1;
    wire Bit4TriggOut2;
    wire Bit4TriggOut3;
    wire Bit4TriggOut4;
    wire Bit2TriggOut;
    
    wire [4:0] DecCountAndDOT0;
    wire [4:0] DecCountAndDOT1;
    wire [4:0] DecCountAndDOT2;
    wire [4:0] DecCountAndDOT3;
    
    //wire that deals with the output of the mux
    wire [4:0] MuxOut;
    
    //wires for the counts of the specific 4 counters that go into the mux
    wire [3:0] Bit4Count1;
    wire [3:0] Bit4Count2;
    wire [3:0] Bit4Count3;
    wire [3:0] Bit4Count4;

    wire [1:0] StrobeCount;



    //the 17 bit counter
    Generic_Counter # (.COUNTER_WIDTH(17),
                      .COUNTER_MAX(99999)
                      )
                      Bit17Counter (
                      .CLK(CLK),
                      .RESET(1'b0),
                      .ENABLE_IN(1'b1),
                      .DIRECT(1'b1),  //direct is connected to constant(in this case the count up version)
                      .TRIG_OUT(Bit17TriggOut)
                      );
 
 //the 4 bit counter0
                      Generic_Counter # (.COUNTER_WIDTH(4),
                                           .COUNTER_MAX(9)
                                           )
                                           Bit4Counter0 (
                                           .CLK(CLK),
                                           .RESET(RESET),
                                           .ENABLE_IN(Bit17TriggOut & ENABLE_IN), // this is a combination of the trigg out of the 17bit counter with the enable in through a AND gate
                                           .DIRECT(1'b1),    //direct is connected to constant(in this case the count up version)
                                           .TRIG_OUT(Bit4TriggOut0) //passes it's trigg out onto the next 4 bit counter
                                           );
 
 //the 4 bit counter1
                      Generic_Counter # (.COUNTER_WIDTH(4),    
                                            .COUNTER_MAX(9)    
                                            )
                                            Bit4Counter1 (
                                            .CLK(CLK),
                                            .RESET(RESET),
                                            .ENABLE_IN(Bit4TriggOut0), //the enable in is now the previous counters trigg out, this concept repeats for the remaining 4 bit counters
                                            .DIRECT(DIRECT),  //direct is connected to itself to pull through the previous value to determine whether it must count up or down
                                            .TRIG_OUT(Bit4TriggOut1),
                                            .COUNT(Bit4Count1)
                                            );    
                       
//the 4 bit counter2
                        Generic_Counter # (.COUNTER_WIDTH(3),      //counter width adjusted for max value
                                              .COUNTER_MAX(5)     //max is adjusted down from 9
                                              )
                                              Bit4Counter2 (
                                              .CLK(CLK),
                                              .RESET(RESET),
                                              .ENABLE_IN(Bit4TriggOut1),
                                              .DIRECT(DIRECT),
                                              .TRIG_OUT(Bit4TriggOut2),
                                              .COUNT(Bit4Count2)
                                              ); 

//the 4 bit counter3
                        Generic_Counter # (.COUNTER_WIDTH(4), 
                                              .COUNTER_MAX(9) 
                                              )
                                              Bit4Counter3 (
                                              .CLK(CLK),
                                              .RESET(RESET),
                                              .ENABLE_IN(Bit4TriggOut2),
                                              .DIRECT(DIRECT),
                                              .TRIG_OUT(Bit4TriggOut3),
                                              .COUNT(Bit4Count3)
                                              ); 

//the 4 bit counter4
                        Generic_Counter # (.COUNTER_WIDTH(3), //counter width adjusted for max value
                                              .COUNTER_MAX(5) //max is adjusted down
                                              )
                                              Bit4Counter4 (
                                              .CLK(CLK),
                                              .RESET(RESET),
                                              .ENABLE_IN(Bit4TriggOut3),
                                              .DIRECT(DIRECT),
                                              .COUNT(Bit4Count4)
                                              ); 

// the 2 bit counter
                    
                        Generic_Counter # (.COUNTER_WIDTH(2),
                                              .COUNTER_MAX(3)
                                              )
                                              Bit2Counter (
                                              .CLK(CLK),
                                              .ENABLE_IN(Bit17TriggOut), //recieves the trigg out of the 17bit as its enable 
                                              .TRIG_OUT(Bit2TriggOut),
                                              .COUNT(StrobeCount) //sends its count out to specifically deal with the segments of the display for the following 7seg code
                                              ); 

//ties each of the counter outputs with a single bit that represents DOT state

assign DecCountAndDOT0 = {1'b0, Bit4Count1};
assign DecCountAndDOT1 = {1'b0, Bit4Count2}; 
assign DecCountAndDOT2 = {1'b1, Bit4Count3};    //this specific one is different as it deals with displaying the decimal point for the MM.SS format (current clock works in SS.mssm but will be fixed)!!
assign DecCountAndDOT3 = {1'b0, Bit4Count4};                       
                    
//instantiate the MUX
Multiplexer_4way Mux4(
               .CONTROL(StrobeCount),
               .IN0(DecCountAndDOT0),
               .IN1(DecCountAndDOT1),
               .IN2(DecCountAndDOT2),
               .IN3(DecCountAndDOT3),
               .OUT(MuxOut)
               );
               
               
//instantiate the 7 seg decoder
  seven_seg Seg7(
              .SEG_SELECT_IN(StrobeCount),
              .x(MuxOut[3:0]),
              .dot(MuxOut[4]),
              .an(SEG_SELECT),
              .seg(DEC_OUT)
              );
                           
                                            
    
    
endmodule
