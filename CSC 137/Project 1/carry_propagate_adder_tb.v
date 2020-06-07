`timescale 1ns/100ps
`include "carry_propagate_adder.v"

module carry_propagate_adder_tb();

reg [2:0] a, b;
reg cin;
wire [2:0] sum;
wire co;


adder_3bits AB1(a, b, cin, sum, co);

initial begin
$display("--------------------------");
a=3'b001; b=3'b001; cin=1'b0;
#1 $display("|%4d | a = %b %b %b | b = %b %b %b | ci = %b | co = %b | s = %b %b %b |", $time, a[2], a[1], a[0], b[2], b[1], b[0], cin, co, sum[2], sum[1], sum[0]);
a=3'b011; b=3'b010; cin=1'b1;
#1 $display("|%4d | a = %b %b %b | b = %b %b %b | ci = %b | co = %b | s = %b %b %b |", $time, a[2], a[1], a[0], b[2], b[1], b[0], cin, co, sum[2], sum[1], sum[0]);
a=3'b011; b=3'b100; cin=1'b0;
#1 $display("|%4d | a = %b %b %b | b = %b %b %b | ci = %b | co = %b | s = %b %b %b |", $time, a[2], a[1], a[0], b[2], b[1], b[0], cin, co, sum[2], sum[1], sum[0]);
a=3'b111; b=3'b001; cin=1'b0;
#1 $display("|%4d | a = %b %b %b | b = %b %b %b | ci = %b | co = %b | s = %b %b %b |", $time, a[2], a[1], a[0], b[2], b[1], b[0], cin, co, sum[2], sum[1], sum[0]);
$display("--------------------------");
end
endmodule
