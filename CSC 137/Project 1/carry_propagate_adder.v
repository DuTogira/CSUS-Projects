`include "full_adder.v"

module adder_3bits
(
input [2:0] a, b,
input cin,
output [2:0] sum,
output co
);

wire w1, w2;

full_adder fa1(a[0], b[0], cin, w1, sum[0]);
full_adder fa2(a[1], b[1], w1, w2, sum[1]);
full_adder fa3(a[2], b[2], w2, co, sum[2]);

endmodule
