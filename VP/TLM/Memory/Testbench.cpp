#include <systemc.h>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "memory.h"
using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

struct TestBench:sc_module{
    simple_initiator_socket<TestBench>tb_socket;
    
    SC_CTOR(TestBench):tb_socket("Socket"){
        SC_THREAD(run);
    }

    void run(){
        unsigned char buff1[Memory::SIZE];
        unsigned char buff2[Memory::SIZE];

        for(int i=0;i<Memory::SIZE;i++){
            buff1[i]=i;
            
        }
        for(int i=0;i<Memory::SIZE;i++){
            tlm_generic_payload trans;
            trans.set_command(TLM_WRITE_COMMAND);
            trans.set_address(i);
            trans.set_data_length(1);
            trans.set_dmi_allowed(0);
            trans.set_streaming_width(1);
            trans.set_byte_enable_ptr(0);
            trans.set_data_ptr(&buff1[i]);
            trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

           
            sc_time delay=sc_time(10,SC_NS);
            tb_socket->b_transport(trans,delay);
            if(trans.is_response_error())
                SC_REPORT_ERROR("TLM-2","ERROR in response");
        }
        bool match=true;
        for(int i=0;i<Memory::SIZE;i++){
            tlm_generic_payload trans;
            trans.set_command(TLM_READ_COMMAND);
            trans.set_address(i);
            trans.set_data_ptr(&buff2[i]);
            trans.set_data_length(1);
            trans.set_byte_enable_ptr(0);
            trans.set_streaming_width(1);
            trans.set_dmi_allowed(true);
            trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

            //printf("Buffer_2=%d\n",buff2[i]);
            sc_time delay=sc_time(10,SC_NS);
            tb_socket->b_transport(trans,delay);
            printf("Buffer_1=%d and Buffer_2=%d\n",buff1[i],buff2[i]);
            if(buff1[i]!=buff2[i]){
                printf("Error address=%d data %d %d\n",i,buff1[i],buff2[i]);
                match=false;
                break;
            }
            if(trans.is_response_error())
                SC_REPORT_ERROR("TLM-2","Response error from b_transport");            

        }
        if(match)
            cout<<"Memory content matched"<<endl;
        else    
            cout<<"Memory content does not match"<<endl;
    }
};

int sc_main(int argc,char *argv[]){
    Memory mem("Memory");
    TestBench tb("TestBench");

    tb.tb_socket.bind(mem.target_socket);

    sc_start();
    return 0;
}