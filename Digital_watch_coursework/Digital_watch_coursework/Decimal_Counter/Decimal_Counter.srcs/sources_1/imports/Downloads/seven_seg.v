`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 14.04.2023 05:04:06
// Design Name: 
// Module Name: seven_seg
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


module seven_seg(
    input [3:0] x,          
    input [1:0] SEG_SELECT_IN,          //  input that tells the module which digit to light up
    input dot,
    output reg [7:0] an,    
    output reg [7:0] seg   
    );


always @ (SEG_SELECT_IN) 
    begin
    case(SEG_SELECT_IN)
        8'h0: an = 8'b1111_1110;
        8'h1: an = 8'b1111_1101;
        8'h2: an = 8'b1111_1011;
        8'h3: an = 8'b1111_0111;
        default: an = 8'b1111_1111;
    
    endcase
    
    // switch case depending on input x: assigns which segments should light up
    case(x)
//           Segment: abcdefg
        4'h0: seg =7'b0000001;
        4'h1: seg =7'b1001111;
        4'h2: seg =7'b0010010;
        4'h3: seg =7'b0000110;
        4'h4: seg =7'b1001100;
        4'h5: seg =7'b0100100;
        4'h6: seg =7'b0100000;
        4'h7: seg =7'b0001111;
        4'h8: seg =7'b0000000;
        4'h9: seg =7'b0000100;
        default: seg =7'b1111111;   // cases from 10 to 15 keep all the segments OFF
                                    // as only nr 0->9 can be displayed per 1 digit
      
            
    endcase
    
    seg[7] = dot ? 0:1;  // ternery if dot high light up the dot
    end
endmodule
