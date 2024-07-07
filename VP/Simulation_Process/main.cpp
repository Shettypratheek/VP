#include <systemc>

using namespace sc_core;
using namespace std;

SC_MODULE(PROCESS){
    sc_clock clk;
    SC_CTOR(PROCESS):clk("clk",1,SC_SEC){
        SC_METHOD(method);
        //SC_THREAD(thread);
        SC_CTHREAD(cthread,clk);
    }

    void method(void){
        cout<<"method triggered @ "<<sc_time_stamp()<<endl;
        next_trigger(sc_time(2,SC_SEC));
    }

    void thread(){
        while(true){
            cout<<"Thread triggered @ "<<sc_time_stamp()<<endl;
            wait(1,SC_SEC);
        }
    }

    void cthread(){
        while(true){
            cout<<"cthread triggered @ "<<sc_time_stamp()<<endl;
            wait();
        }
    }
};

int sc_main(int argc,char*argv[]){

    PROCESS process("PROCESS");
    cout<<"execution phase begins @ "<<sc_time_stamp()<<endl;
    sc_start(2,SC_SEC);
    cout<<"execution phase ends @ "<<sc_time_stamp()<<endl;
    return 0;
}