#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm>

using namespace std;
using namespace tlm;
using namespace tlm_utils;

using namespace sc_core;

struct Memory : sc_module
{
    simple_target_socket<Memory> target_socket;
    enum
    {
        SIZE = 256
    };

    unsigned char *mem;

    SC_CTOR(Memory) : target_socket("Socket")
    {
        target_socket.register_b_transport(this, &Memory::b_transport);
        target_socket.register_transport_dbg(this, &Memory::transport_dbg);
        target_socket.register_get_direct_mem_ptr(this, &Memory::get_direct_mem_ptr);
        mem = new unsigned char[SIZE];
        memset(mem, 0, SIZE);
    }

    virtual void b_transport(tlm_generic_payload &trans, sc_time &delay)
    {
        tlm_command cmd = trans.get_command();
        uint64_t addr = trans.get_address();
        unsigned char *ptr = trans.get_data_ptr();
        unsigned int len = trans.get_data_length();

        if (addr + len > SIZE)
        {
            SC_REPORT_ERROR("TLM-2", " address mismatch");
        }
       // printf("address= %ld and data %d\n",addr,*ptr);
        if (cmd == TLM_WRITE_COMMAND)
            memcpy(&mem[addr], ptr, len);
        else if (cmd == TLM_READ_COMMAND){
            memcpy(ptr,&mem[addr],  len);
            
        }
        trans.set_response_status(TLM_OK_RESPONSE);
    }

    virtual unsigned int transport_dbg(tlm_generic_payload &trans)
    {
        tlm_command cmd=trans.get_command();
        uint64_t addr=trans.get_address();
        unsigned char*ptr=trans.get_data_ptr();
        unsigned int len=trans.get_data_length();
        
        if(addr +len >SIZE){
            SC_REPORT_ERROR("TLM-2","address mismatch");
            return 0;
        }
        if(cmd==TLM_WRITE_COMMAND)
            memcpy(ptr,&mem[addr],len);
        else if(cmd==TLM_READ_COMMAND)
            memcpy(&mem[addr],ptr,len);
        trans.set_response_status(TLM_OK_RESPONSE);

        return len;
            }
    virtual bool get_direct_mem_ptr(tlm_generic_payload &trans,tlm_dmi &dmi_data){
        dmi_data.allow_read_write();
        dmi_data.set_dmi_ptr(mem);
        dmi_data.set_start_address(0);
        dmi_data.set_end_address(SIZE-1);
        dmi_data.set_read_latency(SC_ZERO_TIME);
        dmi_data.set_write_latency(SC_ZERO_TIME);
        return true;
            }
};

#endif