#include <systemc.h>
#include "counter.h"


SC_MODULE(TB_Counter){
    Counter *counter_module;


    SC_CTOR(TB_Counter){
        counter_module=new Counter("Counter");

        SC_THREAD(trigger_counter_event);
        sc_trace_file *Tf=sc_create_vcd_trace_file("Traces");
    sc_trace(Tf,counter_module->counter_event,"Event");
    sc_trace(Tf,counter_module->count,"Counter_Value");
    }

    void trigger_counter_event(){
        
        wait(10,SC_NS);
        cout<<"Triggering event at time "<<sc_time_stamp()<<endl;
        counter_module->trigger_event();

        wait(20,SC_NS);
        cout<<"Triggering event at time "<<sc_time_stamp()<<endl;
        counter_module->trigger_event();

        wait(30,SC_NS);
        cout<<"Triggering event at time "<<sc_time_stamp()<<endl;
        counter_module->trigger_event();

        wait(40,SC_NS);
        cout<<"Triggering event at time "<<sc_time_stamp()<<endl;
        counter_module->trigger_event();
    
        sc_stop();
    }
};

int sc_main(int argc,char *argv[]){
    TB_Counter tb("TB_Counter");
    sc_start();
    return 0;
}