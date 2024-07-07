#include <systemc.h>
#include <iomanip>
using namespace std;
 
 #include "TimerCounter.h"

 SC_MODULE(TB_TimerCounter){
    sc_signal<bool>reset;
    sc_signal<sc_uint<8>>address;
    sc_signal<sc_uint<8>>data_in;
    sc_signal<sc_uint<8>>data_out;

    sc_signal<bool>write_en;
    sc_signal<bool>read_en;
    sc_signal<bool>irq0;
    sc_signal<bool>irq1;

    sc_clock clock;

    TimerCounter *timercounter;

    void test(){
        reset.write(true);
        wait(1,SC_NS);
        reset.write(false);
       
        cout<<"Test case 1: Enable Timer and set Comapare value"<<endl;

        address.write(0x00);
        data_in.write(0x07);
        write_en.write(true);
        wait(1,SC_NS);
        write_en.write(false);

        address.write(0x08);
        data_in.write(0x10);
        write_en.write(true);
        wait(1,SC_NS);
        write_en.write(false);

        for(int i=0;i<20;i++){
            read_en.write(true);
            address.write(0x04);
           wait(1,SC_NS);
            cout<<setw(4)<<sc_time_stamp()<<"      |Count: "<<setw(3)<<data_out.read()<<"     Interrupt0: "<<irq0.read()<<"   |Interrupt1: "<<irq1.read()<<endl;
            read_en.write(false);
           
        }


        cout<<"Test Case 2: Disable Timer: "<<endl;
        address.write(0x00);
        data_in.write(0x00);
        write_en.write(true);
        wait(1,SC_NS);
        write_en.write(false);

        for(int i=0;i<30;i++){
            address.write(0x04);
            read_en.write(true);
            wait(1,SC_NS);
            read_en.write(false);
            cout<<sc_time_stamp()<<"     |Count: "<<data_out.read()<<"      Interrupt0: "<<irq0.read()<<"     |Interrupt1: "<<irq1.read()<<endl;
        }

         cout<<"Test case 3: Enable Timer and check for overflow interrupt"<<endl;

        address.write(0x00);
        data_in.write(0x07);
        write_en.write(true);
        wait(1,SC_NS);
        write_en.write(false);

        address.write(0x08);
        data_in.write(0xFF);
        write_en.write(true);
        wait(1,SC_NS);
        write_en.write(false);

        for(int i=0;i<260;i++){
            address.write(0x04);
            read_en.write(true);
            wait(1,SC_NS);
            read_en.write(false);
            cout<<setw(4)<<sc_time_stamp()<<"      |Count: "<<setw(3)<<data_out.read()<<"     Interrupt0: "<<irq0.read()<<"   |Interrupt1: "<<irq1.read()<<endl;
        }

        cout<<" Check for reset Functionality"<<endl;

        reset.write(true);
        wait(1,SC_NS);
        reset.write(false);
        for(int i=0;i<260;i++){
            address.write(0x04);
            read_en.write(true);
            wait(1,SC_NS);
            read_en.write(false);
            cout<<setw(4)<<sc_time_stamp()<<"      |Count: "<<setw(3)<<data_out.read()<<"     Interrupt0: "<<irq0.read()<<"   |Interrupt1: "<<irq1.read()<<endl;

        }
        sc_stop();

    }

    SC_CTOR(TB_TimerCounter):clock("clock",1,SC_NS){
        timercounter=new TimerCounter("TimerCounter");
        timercounter->clock(clock);
        timercounter->reset(reset);
        timercounter->address(address);
        timercounter->data_in(data_in);
        timercounter->data_out(data_out);

        timercounter->write_en(write_en);
        timercounter->read_en(read_en);
        timercounter->irq0(irq0);
        timercounter->irq1(irq1);

        sc_trace_file* Tf=sc_create_vcd_trace_file("Traces");
        sc_trace(Tf,clock,"clock");
        sc_trace(Tf,reset,"reset");
        sc_trace(Tf,address,"address");
        sc_trace(Tf,data_in,"data_in");
        sc_trace(Tf,data_out,"data_out");
        sc_trace(Tf,write_en,"write_en");
        sc_trace(Tf,read_en,"read_en");
        sc_trace(Tf,irq0,"irq0");
        sc_trace(Tf,irq1,"irq1");
        sc_trace(Tf,timercounter->TIMER_CONTROL_REG,"TIMER_CONTROL_REG");
        sc_trace(Tf,timercounter->TIMER_VALUE_REG,"TIMER_VALUE_REG");
        sc_trace(Tf,timercounter->TIMER_COMPARE_REG,"TIMER_COMPARE_REG");
        sc_trace(Tf,timercounter->TIMER_IRQ_STATUS_REG,"TIMER_IRQ_STATUS_REG_BIT");
        sc_trace(Tf,timercounter->TIMER_CONTROL_CMP_BIT,"TIMER_CONTROL_CMP_BIT");
        sc_trace(Tf,timercounter->TIMER_CONTROL_EN_BIT,"TIMER_CONTROL_EN_BIT");
        sc_trace(Tf,timercounter->TIMER_CONTROL_OV_BIT,"TIMER_CONTROL_OV_BIT");
        SC_THREAD(test);
    }
    ~TB_TimerCounter(){
        delete timercounter;
    }
 };

 int sc_main(int argc,char *argv[]){
    TB_TimerCounter tb("counter");
    sc_start();

    return 0;
 }