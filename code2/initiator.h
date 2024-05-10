#ifndef INI_H
#define INI_H
#include <systemc.h>

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace std;
using namespace sc_core;

struct Initiator:sc_module
{
tlm_utils::simple_initiator_socket<Initiator>socket;
SC_CTOR(Initiator):socket("socket")
{
SC_THREAD(generate_trans);
}

void generate_trans(){
for (uint8_t i=0;i<4;i++)
{
tlm::tlm_generic_payload trans;
sc_time delay=SC_ZERO_TIME;
uint8_t data=i;


trans.set_command(tlm::TLM_WRITE_COMMAND);
trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
trans.set_data_length(sizeof(data));
trans.set_streaming_width(sizeof(data));
trans.set_byte_enable_ptr(0);
trans.set_dmi_allowed(false);
trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

socket->b_transport(trans,delay);
if(trans.is_response_error())
	SC_REPORT_ERROR("TLM","ERROR OCCURED");
wait(1,SC_NS);
}
}
};
#endif
