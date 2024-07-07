#include <systemc.h>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

struct UART:sc_module{
    simple_target_socket<UART>target_socket;
    bool uart_tx;
    bool uart_rx;

    sc_event tx_event;
    sc_event rx_event;

    enum{
        CTRL_REG_OFFSET=0X00,
        RXDATA_REG_OFFSET=0X04,
        TXDATA_REG_OFFSET=0X08
    };
    enum{
        CTRL_ENABLE=1<<0,
        CTRL_START_TRANSFER=1<<6
    };
    uint32_t ctrl_reg;
    uint32_t rxdata_reg;
    uint32_t txdata_reg;

    SC_CTOR(UART): target_socket("Target_Socket"){
        target_socket.register_b_transport(this,&UART::b_transport);

        ctrl_reg=0x00;
        rxdata_reg=0x00;
        txdata_reg=0x00;

        SC_THREAD(transmitting_process);
        SC_THREAD(receiving_process);
    }

    void b_transport(tlm_generic_payload&trans,sc_time &delay){
        
        tlm_command cmd=trans.get_command();
        uint64_t addr=trans.get_address();
        unsigned char*data_ptr=trans.get_data_ptr();
        unsigned int length=trans.get_data_length();
        
        if(cmd==TLM_WRITE_COMMAND){
            if(addr==CTRL_REG_OFFSET){
                memcpy(&ctrl_reg,data_ptr,length);
                if(ctrl_reg &CTRL_START_TRANSFER)
                    tx_event.notify(SC_ZERO_TIME);
            }
            else if(addr==RXDATA_REG_OFFSET){
                memcpy(&txdata_reg,data_ptr,length);
                if(ctrl_reg &CTRL_START_TRANSFER)
                    tx_event.notify(SC_ZERO_TIME);

            }
            else if (cmd==TLM_READ_COMMAND){
                if(addr==RXDATA_REG_OFFSET)
                    memcpy(data_ptr,&rxdata_reg,length);
            }

            trans.set_response_status(TLM_OK_RESPONSE);

        }

       

    }
    void transmitting_process(){
        while(true){
            wait(tx_event);
            if(ctrl_reg & CTRL_ENABLE){
                
            }
        }
    }
    void receiving_process(){}

    
};