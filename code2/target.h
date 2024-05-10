#ifndef TAR_H
#define TAR_H

#include <systemc.h>

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace std;
using namespace sc_core;

struct Target:sc_module
{
tlm_utils::simple_target_socket<Target>socket;
SC_CTOR(Target){
socket.register_b_transport(this,&Target::decoder);
}
virtual void decoder(tlm::tlm_generic_payload &trans,sc_time &delay)
{
uint8_t *data=trans.get_data_ptr();
cout<<"@ "<<sc_time_stamp()<<"Decoder received data: "<<static_cast<unsigned>(*data)<<endl;
delay+=sc_time(10,SC_NS);

trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
};

#endif
