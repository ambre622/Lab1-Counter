#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
  int i;
  int clk;

  Verilated::commandArgs(argc, argv);
  // init top verilog instance 
  Vtop* top = new Vtop; //instanciate the counter module as Vcounter 
  // init trace dump
  Verilated::traceEverOn(true); // turn on signal tracing and tell the verilator to dump the waveform data to counter.vcd
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top -> trace (tfp, 99);
  tfp -> open ("counter.vcd");

  //initialize Vbuddy
  if (vbdOpen()!=1) return(-1);
  vbdHeader("Single Step Counter");
  vbdSetMode(1);

  // initialize simulation inputs 
  top -> clk = 1;  // set initial signal levels. top is the name of the top level entity
  top -> rst = 1;  // only top level signals are visible
  top -> step = 0;



  // run sumulation for many clock cycles
  for (i=0; i<2000; i++) { // for loop where the simulation happens. i counts the clock cycles 
    //dump variables into VCD file and toggle clock

    for (clk=0; clk<2; clk++) { // toggles the clock
      tfp -> dump (2*i+clk);
      top -> clk = !top -> clk;
      top -> eval ();
    }

    //vbdPlot(int(top->count_out) & 0xFF, 0, 255);
    vbdHex(3, (int(top->bcd) >> 8) & 0xF);   // hundreds
    vbdHex(2, (int(top->bcd) >> 4) & 0xF);   // tens
    vbdHex(1,  int(top->bcd)       & 0xF);   // ones

    vbdCycle(i+1);


    top ->rst = (i<2) | (i == 15); 
    top->step = vbdFlag();
    if (Verilated::gotFinish()) exit(0);
  }

  vbdClose();
  tfp -> close();
  exit(0);
}