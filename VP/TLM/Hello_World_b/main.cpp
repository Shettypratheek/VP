#include <systemc.h>
#include    <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

using namespace std;
using namespace tlm;
using namespace sc_core;

struct Initiator: sc_module{
    tlm_utils::simple_initiator_socket<Initiator>socket;

    SC_CTOR(Initiator):socket("socket"){
        SC_THREAD(thread_process);
    }

    void thread_process(){
        tlm_generic_payload trans;
        sc_time delay=SC_ZERO_TIME;

   const char *data="Hello World";
        unsigned int data_length=strlen(data)+1;

        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(0x00);
        trans.set_data_length(data_length);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(const_cast<char*>(data)));

        socket->b_transport(trans,delay);

        if(trans.is_response_error())
                SC_REPORT_ERROR("TLM-2","Response error from b_transport");

        cout<<"Transaction completed: "<<data<<endl;
    }
};

struct Target: sc_module{
    tlm_utils::simple_target_socket<Target> socket;

    SC_CTOR(Target):socket("Socket"){
        socket.register_b_transport(this,&Target::receiving_data);
    }

    void receiving_data(tlm_generic_payload &trans,sc_time&delay){
        tlm_command cmd=trans.get_command();
        uint64_t addr=trans.get_address();
     unsigned   char*data_ptr=trans.get_data_ptr();
        int data_length=trans.get_data_length();

        wait(delay);

        if(cmd==TLM_WRITE_COMMAND){
            cout<<" Write transaction received at address "<<addr<<endl;
            cout<<" Data: "<<data_ptr<<endl;
        }

        trans.set_response_status(TLM_OK_RESPONSE);
    }
};

SC_MODULE(Top){
    Initiator * initiator;
    Target *target;

    SC_CTOR(Top){
        initiator=new Initiator("initiator");
        target=new Target("Target");

        initiator->socket.bind(target->socket);
    }

    ~Top(){
        delete initiator;
        delete target;
    }
};

int sc_main(int argc,char *argv[]){
    Top top("Top");
    sc_start();
    return 0;
}