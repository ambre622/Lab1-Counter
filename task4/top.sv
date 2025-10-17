module top #(
  parameter WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             step,     // step
  output logic [11:0]      bcd,      // count output
  output logic [WIDTH-1:0] count_out
);

  logic  [WIDTH-1:0]       count;    // interconnect wire

counter myCounter (
  .clk (clk),
  .rst (rst),
  .step (step),
  .count (count)
);

assign count_out = count;

bin2bcd myDecoder (
  .x (count),
  .BCD (bcd)
);

endmodule
