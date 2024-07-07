#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "timer_counter.h"

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

struct TB_TimerCounter:sc_module{
    simple_initiator_socket<TB_TimerCounter>socket;

    sc_out<sc_time>clock;
    sc_out<bool>reset;
    sc_in<bool>irq0,irq1;

    SC_CTOR(TB_TimerCounter):socket("socket"){
        SC_THREAD(test);
    }
    private:    
        sc_uint<8>data_out;
    void test(){
        reset.write(1);
        wait(clock.read());
        reset.write(0);

        write(0x00,0x07);
        write(0x08,0x012);
        // for(int i=0;i<30;i++){
        //     wait(10,SC_NS);
        //     read(0x04);
        //     cout<<"Count: "<<data_out<<" IRQ0 "<<irq0<<" IRQ1 "<<irq1<<" at time "<<sc_time_stamp()<<endl;
        // }
        sc_stop();
    }

    void write(uint64_t addr,sc_uint<8>data){
        tlm_generic_payload trans;
        sc_time delay=SC_ZERO_TIME;

        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_streaming_width(1);
        trans.set_byte_enable_ptr(0);
        trans.set_data_length(1);
        trans.set_dmi_allowed(0);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

        unsigned char data_array[1];
        //memcpy(data_array,&data,sizeof(data));
      
       
        trans.set_data_ptr(data_array);
       
        socket->b_transport(trans,delay);

        if(trans.is_response_error())
            SC_REPORT_ERROR("tlm-2","  Response error from b_transport");

    }
    void read(uint64_t addr){
        tlm_generic_payload trans;
        sc_time delay=SC_ZERO_TIME;

        trans.set_command(TLM_READ_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(1);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(0);
        trans.set_dmi_allowed(0);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

        unsigned char data_array[4];
        trans.set_data_ptr(data_array);

        socket->b_transport(trans,delay);

        if(trans.is_response_error())
            SC_REPORT_ERROR("TLM","Response has error");
    
        data_out=data_array[0];

            }
};
int sc_main(int argc,char *argv[]){
    Target timer("Timer");
    TB_TimerCounter tb("TestBench");

    tb.socket.bind(timer.timer_socket);

    sc_signal<sc_time>clock("clock");
    sc_signal<bool>reset;
    sc_signal<bool>irq0,irq1;


    timer.reset(reset);
    timer.clock(clock);
    timer.irq0(irq0);
    timer.irq1(irq1);


    tb.reset(reset);
    tb.clock(clock);
    tb.irq0(irq0);
    tb.irq1(irq1);

    clock.write(sc_time(10,SC_NS));
    sc_start();

    return 0;



    
}