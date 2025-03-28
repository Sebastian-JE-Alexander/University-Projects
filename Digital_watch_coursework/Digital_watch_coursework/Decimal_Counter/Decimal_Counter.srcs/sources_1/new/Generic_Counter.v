`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.10.2023 10:10:42
// Design Name: 
// Module Name: Generic_Counter
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


module Generic_Counter(
       CLK,
       RESET,
       ENABLE_IN,
       DIRECT,
       TRIG_OUT,
       COUNT
 );
     //sets the generic width and max to 4 and 9 respectively but is easily adjusted in the wrapper for specific cases
     parameter COUNTER_WIDTH = 4;
     parameter COUNTER_MAX   = 9;
     
     input     CLK;
     input     RESET;
     input     ENABLE_IN;
     input     DIRECT;    //input that determines whether clock counts up or down
     output    TRIG_OUT;
     output    [COUNTER_WIDTH-1:0] COUNT;
     
     //declare reg that hold current count value and trigger out
     //between clock cycles
     
     reg [COUNTER_WIDTH-1:0] count_value;
     reg Trigger_out;
     
     
     //Synchronous logic of count_value
     always@(posedge CLK) begin
        if(RESET)
          count_value <= 0;
        else begin
          if(ENABLE_IN && DIRECT) begin   //if enable in and direction is 1 (count up)
             if(count_value == COUNTER_MAX)
                  count_value <= 0;
             else
                 count_value <= count_value + 1;    
           end
           
          else if(ENABLE_IN && !DIRECT) begin  //if enable in and direction is 0 (count down)
              if(count_value == 0)
                   count_value <= COUNTER_MAX;
              else
                  count_value <= count_value - 1;    
            end
       end
   end
   
   //synchronous logic for Trigger_out
   always@(posedge CLK) begin
        if(RESET)
           Trigger_out <= 0;
        else begin
             if(ENABLE_IN && (count_value == COUNTER_MAX))
                Trigger_out <= 1;
             else
                Trigger_out <= 0;
         end
       end
       
   //assignment that ties the count_value and Trigger_out to COUNT and TRIG_OUT
   assign COUNT    = count_value;
   assign TRIG_OUT = Trigger_out; 
   
endmodule
