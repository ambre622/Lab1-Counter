#include "Vcounter.h"          // Generated header for the module counter
#include "verilated.h"         // Verilator core
#include "verilated_vcd_c.h"   // VCD tracing

int main(int argc, char **argv, char **env) {
    int i;                     // i counts the number of clock cycles to simulate.
    int clk;

    Verilated::commandArgs(argc, argv);

    Vcounter* top = new Vcounter;       // Instantiate the counter module as Vcounter. This is the DUT.

    Verilated::traceEverOn(true);       // Turn on signal tracing, and tell Verilator to dump the waveform data to counter.vcd
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // Initialize simulation inputs (only top-level signals are visible)
    top->clk = 1;
    top->rst = 1;
    top->en  = 0;
    int pause_cycles = 0;
    bool in_pause = false;

    // Run simulation for many clock cycles
    for (i = 0; i < 300; i++) {      // This is the for-loop where simulation happens. i counts the clock cycles.

        //1 full clk cycle
        // Dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump (2*i + clk);        // unit is in ps!!!
            top->clk = !top->clk;        // Toggle the clock
            top->eval();                 // Evaluate on both edges
        }

        //release to pause
        top->rst = (i < 2);
        // if we've just reached 0x9, start a 3-cycle pause (only once)
        if (!in_pause && (top->count == 0x9)) {
            pause_cycles = 3;     // pause *next* 3 cycles
            in_pause = true;
        }

        // drive enable for next cycle
        if (i < 2) {
            top->en = 0;                // keep disabled during reset window
        } else if (pause_cycles > 0) {
            top->en = 0;                // paused
            --pause_cycles;
        } else {
            top->en = 1;                // normal counting
        }

        // Change rst and en signals during simulation
        //top->rst = (i < 2) | (i == 15); // Assert reset for first 2 cycles and once again at cycle 15
        //top->en  = (i > 4);              // Enable counting after a few cycles

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}