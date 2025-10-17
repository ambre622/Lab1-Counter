module counter #(
  parameter WIDTH = 8
)(
  //interface signals
  input  logic              clk,
  input  logic              rst,
  input  logic              step,
  output logic [WIDTH-1:0]  count
);

always_ff @ (posedge clk or posedge rst)
  if(rst) count <= {WIDTH{1'b0}};
  else    count <= step ? count + {{WIDTH-1{1'b0}},1'b1} : count;
endmodule
