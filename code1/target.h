#ifndef TAR_H
#define TAR_H
#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace std;
struct Target:sc_module
{
tlm_utils::simple_target_socket<Target>socket;
SC_CTOR(Target):socket("socket")
{
socket.register_b_transport(this,&Target::receive_hello);
}
virtual void receive_hello(tlm::tlm_generic_payload &trans,sc_time &delay)
{
unsigned char *data=trans.get_data_ptr();
unsigned int data_length=trans.get_data_length();

tlm::tlm_command cmd=trans.get_command();
if(cmd==tlm::TLM_WRITE_COMMAND){
cout<<"Received data:";
for(int i=0;i<data_length;i++)
	cout<<data[i];
trans.set_response_status(tlm::TLM_OK_RESPONSE);
	
}
else
	trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
}
};
#endif
