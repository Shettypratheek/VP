#ifndef _COUNTER_H
#define _COUNTER_H

#include <systemc.h>

SC_MODULE(Counter){
    sc_event counter_event;

    sc_uint<8>count;

    SC_CTOR(Counter){
        count=0;
        SC_THREAD(counter_process);
    }

    void counter_process(){
        while(true){
        wait(counter_event);
        count++;
        cout<<"Counter: "<<count<<" at time "<<sc_time_stamp()<<endl;
        }
    }

    void trigger_event(){
        counter_event.notify();
    }
};
#endif