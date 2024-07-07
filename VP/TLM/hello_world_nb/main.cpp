#include <systemc.h>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

using namespace std;
using namespace sc_core;
using namespace tlm;

struct Initiator: sc_module{
    tlm_utils::simple_initiator_socket<Initiator>socket;
    SC_CTOR(Initiator):socket("Socket"){
        SC_THREAD( thread_process);
    }
    void thread_process(){
        tlm_generic_payload trans;
        sc_time delay=SC_ZERO_TIME;
        tlm_phase phase;

      const  char *data="Hello World";
        int data_length=strlen(data)+1;

        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(0x01);
        trans.set_data_length(data_length);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(const_cast<char*>(data)));

        tlm_sync_enum result;
        phase=BEGIN_REQ;
        sc_time local_time=SC_ZERO_TIME;

        result=socket->nb_transport_fw(trans,phase,delay);

        if(result==TLM_COMPLETED){
            if(trans.is_response_error())
                SC_REPORT_ERROR("TLM-2","Response error from nb_transport_fw");
            else
                cout<<"Transaction completed with data "<<data<<endl;
        }
        else if(result==TLM_UPDATED)
            cout<<"Transaction in progress "<<endl;
        else if(result==TLM_ACCEPTED)
            cout<<"Transaction accepted "<<endl;
    }
};

struct Target:sc_module{
    tlm_utils::simple_target_socket<Target>socket;
    SC_CTOR(Target):socket("Socket"){
        socket.register_nb_transport_fw(this,&Target::processing);
    }
    tlm_sync_enum processing(tlm_generic_payload & trans, tlm_phase &phase,sc_time &delay){
        tlm_command cmd=trans.get_command();
        uint64_t addr=trans.get_address();
        int data_length=trans.get_data_length();
       unsigned char *data=trans.get_data_ptr();

        wait(delay);

        if(cmd==TLM_WRITE_COMMAND){
            cout<<"Write Transaction received at address "<<addr<<endl;
            cout<<"Data "<<endl;
        }

        trans.set_response_status(TLM_OK_RESPONSE);
        return TLM_COMPLETED;
    }
};

SC_MODULE(Top){
    Initiator *initiator;
    Target *target;
    SC_CTOR(Top){
        initiator=new Initiator("Initiator");
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