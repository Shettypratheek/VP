#include <systemc.h>
#include<iomanip>
#include "TimerCounter.h"

using namespace std;
SC_MODULE(TB_TimerCounter){
    sc_signal<sc_time>clock;
    sc_signal<bool>reset;
    sc_signal<bool>write_en;
    sc_signal<bool>read_en;
    sc_signal<sc_uint<8>>address;
    sc_signal<sc_uint<8>>data_in;
    sc_signal<sc_uint<8>>data_out;
    sc_signal<bool>irq0;
    sc_signal<bool>irq1;

    TimerCounter *timercounter;

    SC_CTOR(TB_TimerCounter){
        timercounter=new TimerCounter("Timer");

        timercounter->clock(clock);
        timercounter->reset(reset);
        timercounter->write_en(write_en);
        timercounter->read_en(read_en);
        timercounter->address(address);
        timercounter->data_in(data_in);
        timercounter->data_out(data_out);
        timercounter->irq0(irq0);
        timercounter->irq1(irq1);

        sc_trace_file *Tf=sc_create_vcd_trace_file("Traces");
        sc_trace(Tf,clock,"clock");
        sc_trace(Tf,reset,"reset");
        sc_trace(Tf,write_en,"write_en");
        sc_trace(Tf,read_en,"read_en");
        sc_trace(Tf,address,"address");
        sc_trace(Tf,data_in,"data_in");
        sc_trace(Tf,data_out,"data_out");
        sc_trace(Tf,irq0,"interrupt0");
        sc_trace(Tf,irq1,"interrupt0");
        sc_trace(Tf,timercounter->CTRL,"Control_Reg");
        sc_trace(Tf,timercounter->TimerValue,"TimerValue");
        sc_trace(Tf,timercounter->TimerCompare,"TimerCompare");
        sc_trace(Tf,timercounter->TimerIRQStatus,"TimerIRQ");

        SC_THREAD(test);
    }

    void test(){

        reset.write(true);
        wait(10,SC_NS);
        reset.write(false);

       
        address.write(0x00);
        data_in.write(0x07);
         write_en.write(true);
        wait(10,SC_NS);
        write_en.write(false);

        
        address.write(0x08);
        data_in.write(0x12);
        write_en.write(true);
        wait(10,SC_NS);
        write_en.write(false);

        for(int i=0;i<30;i++){

        
                        read_en.write(true);
            address.write(0x04);
            wait(10,SC_NS);
            read_en.write(false);

                cout<<"Count: "<<data_out.read()<<" Interrupt0 "<<irq0.read()<<" Interrupt1 "<<irq1.read()<<" at time "<<sc_time_stamp()<<endl;
            
        }

        
        address.write(0x00);
        data_in.write(0x05);
         write_en.write(true);
        wait(10,SC_NS);
        write_en.write(false);
        for(int i=0;i<300;i++){

        
                        read_en.write(true);
            address.write(0x04);
            wait(10,SC_NS);
            read_en.write(false);

                cout<<"Count: "<<data_out.read()<<" Interrupt0 "<<irq0.read()<<" Interrupt1 "<<irq1.read()<<" at time "<<sc_time_stamp()<<endl;
            
        }

        address.write(0x0C);
        data_in.write(0x02);
        write_en.write(true);
        wait(10,SC_NS);
        write_en.write(false);

        read_en.write(true);
            address.write(0x0C);
            wait(10,SC_NS);
            read_en.write(false);
        sc_stop();
    }
};

int sc_main(int argc,char *argv[]){
    TB_TimerCounter tb("Test_Bench");

    sc_start();
    return 0;
}