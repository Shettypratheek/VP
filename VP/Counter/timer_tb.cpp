#include "systemc.h"
#include "timer_counter.h"

SC_MODULE(testbench) {
    sc_signal<sc_time> clk;        // Reset signal
    sc_signal<bool> rst;        // Reset signal
    sc_signal<sc_uint<8>> addr; // Address signal
    sc_signal<sc_uint<8>> data; // Data signal
    sc_signal<bool> wen;        // Write enable signal
    sc_signal<bool> irq0;       // IRQ 0 signal
    sc_signal<bool> irq1;       // IRQ 1 signal

    timer_counter timer_counter_inst; // Timer counter instance

    // Test function to verify the functionality of the timer counter
    void test() {
        // Reset
        rst = false;
        wait(1, SC_NS);
        rst = true;
        cout<<"REG "<<timer_reg[TIMER_VALUE_REG].to_uint()<<endl;

        // Test writing to registers
        addr = TIMER_CONTROL_REG;
        data = 0b000001; // Enable timer
        wen = true;
        wait(1, SC_NS); // count =1
        cout<<"REG "<<timer_reg[TIMER_VALUE_REG].to_uint()<<endl;


        addr = TIMER_COMPARE_REG;
        data = 10; // Set compare value to 10
        wen = true;
        wait(1, SC_NS); // count =2

        addr = TIMER_IRQ_STATUS_REG;
        data = 0; // Clear IRQ status
        wen = true;
        wait(1, SC_NS); // count =3

        // Start timer
        addr = TIMER_CONTROL_REG;
        data = 0b000000111; // Enable timer and compare interrupt
        wen = true;
        wait(1, SC_NS); // count =4
        cout<<"REG "<<timer_reg[TIMER_VALUE_REG].to_uint()<<endl;

        wait(6, SC_NS); // count =10
        // Wait for interrupt

        wait(SC_ZERO_TIME);
        // Check IRQ status
        addr = TIMER_IRQ_STATUS_REG;
        wen = false;
        cout << "Checking IRQ0 status " << irq0.read() << endl;
        cout << "Checking IRQ1 status " << irq1.read() << endl;
        cout << "Checking TimerControl Value " << timer_reg[TIMER_CONTROL_REG] << endl;
        cout << "Checking Timer Value " << timer_reg[TIMER_VALUE_REG].to_int() << endl;
        assert(irq0.read() == true); // Timer compare interrupt asserted
        wait(1, SC_NS);              // count =11

        // Overflow interrupt test
        addr = TIMER_CONTROL_REG;
        data = 0b000000101; // Enable timer and overflow interrupt
        wen = true;
        wait(1, SC_NS); // count =12

        // Wait for overflow
        wait(244, SC_NS); // count =256

        // Check IRQ status
        addr = TIMER_IRQ_STATUS_REG;
        wen = false;
        wait(SC_ZERO_TIME);       
        cout << "Checking IRQ0 status after overflow " << irq0.read() << endl;
        cout << "Checking IRQ1 status after overflow " << irq1.read() << endl;
        assert(irq1.read() == true); // Timer overflow interrupt asserted
        wait(1, SC_NS);              // count =1

        // Stop timer
        addr = TIMER_CONTROL_REG;
        data = 0b0000000000; // Disable timer
        wen = true;
        wait(1, SC_NS); // count =1

        // Wait for a few clock cycles
        wait(10, SC_NS); // count =1

        cout << "Checking Timer Value " << timer_reg[TIMER_VALUE_REG].to_int() << endl;

        assert(timer_reg[TIMER_VALUE_REG] == 1);

        // Check if no interrupts are asserted
        addr = TIMER_IRQ_STATUS_REG;
        wen = false;
        wait(1, SC_NS);
        assert(irq0.read() == false);
        assert(irq1.read() == false);

        cout << "Checking Timer status " << timer_reg[TIMER_IRQ_STATUS_REG] << endl;

        assert(timer_reg[TIMER_IRQ_STATUS_REG] == 0b11);

        addr = TIMER_IRQ_STATUS_REG;
        wen = true;
        data = 0b0000000001; // clear TIMER_IRQ_STATUS_CMP_IRQ_BIT
        wait(1, SC_NS);
        wait(SC_ZERO_TIME);
        cout << "Checking Timer status " << timer_reg[TIMER_IRQ_STATUS_REG] << endl;

        assert(timer_reg[TIMER_IRQ_STATUS_REG] == 0b10);
        addr = TIMER_IRQ_STATUS_REG;
        wen = true;
        data = 0b00000000010; // clear TIMER_IRQ_STATUS_OV_IRQ_BIT
        wait(1, SC_NS);
        cout << "Checking Timer status " << timer_reg[TIMER_IRQ_STATUS_REG] << endl;

        assert(timer_reg[TIMER_IRQ_STATUS_REG] == 0b00);

        // Stop simulation
        sc_stop();
    }

    // Constructor for the testbench
    SC_CTOR(testbench) : timer_counter_inst("timer_counter_inst") {
        // Connect signals to timer counter instance
        clk.write(sc_time(1,SC_NS));
        timer_counter_inst.clock(clk);
        timer_counter_inst.reset(rst);
        timer_counter_inst.address(addr);
        timer_counter_inst.data(data);
        timer_counter_inst.write_enable(wen);
        timer_counter_inst.irq0(irq0);
        timer_counter_inst.irq1(irq1);

        // Open VCD trace file
        sc_trace_file *tf = sc_create_vcd_trace_file("trace");
        // Trace signals
        sc_trace(tf, rst, "rst");
        sc_trace(tf, addr, "addr");
        sc_trace(tf, data, "data");
        sc_trace(tf, wen, "wen");
        sc_trace(tf, irq0, "irq0");
        sc_trace(tf, irq1, "irq1");
        sc_trace(tf, timer_reg[TIMER_CONTROL_REG], "TIMER_CONTROL_REG");
        sc_trace(tf, timer_reg[TIMER_VALUE_REG], "TIMER_VALUE_REG");
        sc_trace(tf, timer_reg[TIMER_COMPARE_REG], "TIMER_COMPARE_REG");
        sc_trace(tf, timer_reg[TIMER_IRQ_STATUS_REG], "TIMER_IRQ_STATUS_REG");

        // Start test function as a thread
        SC_THREAD(test);
    }

};

// Main function
int sc_main(int argc, char* argv[]) {
    testbench tb("tb"); // Instantiate the testbench
    sc_start(1,SC_MS); // Start simulation
    return 0;
}
