#ifndef INI_H
#define INI_H
#include <stdio.h>

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace std;
using namespace sc_core;

struct Initiator:sc_module
{
tlm_utils::simple_initiator_socket<Initiator> socket;
SC_CTOR(Initiator):socket("socket")
{
SC_THREAD(send_hello);
}
void send_hello()
{
tlm::tlm_generic_payload trans;
sc_time delay=sc_time(5,SC_NS);

trans.set_command(tlm::	TLM_WRITE_COMMAND);
char *data=new char[16];
strcpy(data,"Pratheek Shetty");

trans.set_data_ptr(reinterpret_cast<unsigned char*>(data));
trans.set_data_length(16);
socket->b_transport(trans,delay);
if(trans.is_response_error())
	SC_REPORT_ERROR("TLM","ERROR DURING TRANS");

}
};
#endif
