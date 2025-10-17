#include "Vcounter.h"          // Generated header for the module counter
#include "verilated.h"         // Verilator core
#include "verilated_vcd_c.h"   // VCD tracing
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;                     // i counts the number of clock cycles to simulate.
    int clk;

    Verilated::commandArgs(argc, argv);

    Vcounter* top = new Vcounter;       // Instantiate the counter module as Vcounter. This is the DUT.

    Verilated::traceEverOn(true);       // Turn on signal tracing, and tell Verilator to dump the waveform data to counter.vcd
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // --- init Vbuddy ---
    if (vbdOpen() != 1) return -1;   // open and initialize Vbuddy connection (port from vbuddy.cfg)
    vbdHeader("Lab 1: Counter");

    // Initialize simulation inputs (only top-level signals are visible)
    top->clk = 1;
    top->rst = 1;
    top->en  = 0;

    // Run simulation for many clock cycles
    for (i = 0; i < 300; i++) {      // This is the for-loop where simulation happens. i counts the clock cycles.

        //1 full clk cycle
        // Dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump (2*i + clk);        // unit is in ps!!!
            top->clk = !top->clk;        // Toggle the clock
            top->eval();                 // Evaluate on both edges
        }

        // ---- show counter value on Vbuddy 7-seg (hex) ----
        // Four hex digits: [4]=MSN ... [1]=LSN
        //send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i + 1);
        //end of Vbuddy output section

        // Change input stimuli
        top->rst = (i < 2) | (i == 15); // Assert reset for first 2 cycles and once again at cycle 15
        top->en  = vbdFlag();
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();      // close Vbuddy connection
    tfp->close();
    exit(0);
}