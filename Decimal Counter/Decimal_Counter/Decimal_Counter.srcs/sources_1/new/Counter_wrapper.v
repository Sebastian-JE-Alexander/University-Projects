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
   output  [4:0] SEG_SELECT,
   output [7:0] DEC_OUT
    );
    
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
    
    wire [4:0] MuxOut;





    //the 17 bit counter
    Generic_Counter # (.COUNTER_WIDTH(17),
                      .COUNTER_MAX(99999)
                      )
                      Bit17Counter (
                      .CLK(CLK),
                      .RESET(1'b0),
                      .ENABLE_IN(1'b1),
                      .TRIG_OUT(Bit17TriggOut)
                      );
 
 //the 4 bit counter0
 Generic_Counter # (.COUNTER_WIDTH(4),
                       .COUNTER_MAX(9)
                       )
                       Bit4Counter0 (
                       .CLK(CLK),
                       .RESET(RESET),
                       .ENABLE_IN(Bit17TriggOut & ENABLE_IN),
                       .TRIG_OUT(Bit4TriggOut0)
                       );
 
 //the 4 bit counter1
 Generic_Counter # (.COUNTER_WIDTH(4),
                       .COUNTER_MAX(9)
                       )
                       Bit4Counter1 (
                       .CLK(CLK),
                       .RESET(RESET),
                       .ENABLE_IN(Bit4TriggOut0),
                       .TRIG_OUT(Bit4TriggOut1),
                       .COUNT(Bit4Count1)
                       );    
                       
//the 4 bit counter2
                        Generic_Counter # (.COUNTER_WIDTH(4),
                                              .COUNTER_MAX(9)
                                              )
                                              Bit4Counter2 (
                                              .CLK(CLK),
                                              .RESET(RESET),
                                              .ENABLE_IN(Bit4TriggOut1),
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
                                              .TRIG_OUT(Bit4TriggOut3),
                                              .COUNT(Bit4Count3)
                                              ); 

//the 4 bit counter4
                        Generic_Counter # (.COUNTER_WIDTH(4),
                                              .COUNTER_MAX(9)
                                              )
                                              Bit4Counter4 (
                                              .CLK(CLK),
                                              .RESET(RESET),
                                              .ENABLE_IN(Bit4TriggOut3),
                                              .COUNT(Bit4Count4)
                                              ); 

// the 2 bit counter
                    
                    Generic_Counter # (.COUNTER_WIDTH(2),
                                              .COUNTER_MAX(3)
                                              )
                                              Bit2Counter (
                                              .CLK(CLK),
                                              .ENABLE_IN(Bit17TriggOut),
                                              .TRIG_OUT(Bit2TriggOut),
                                              .COUNT(StrobeCount)
                                              ); 

//ties each of the counter outputs with a single bit that represents DOT state

assign DecCountAndDOT0 = {1'b0, Bit4Count1};
assign DecCountAndDOT1 = {1'b0, Bit4Count2}; 
assign DecCountAndDOT2 = {1'b0, Bit4Count3};
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
