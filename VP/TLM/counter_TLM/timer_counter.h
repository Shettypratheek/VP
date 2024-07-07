#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <systemc.h>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

struct Target:sc_module{
    simple_target_socket<Target>timer_socket;

    sc_in<bool>reset;
    sc_in<sc_time>clock;
    sc_out<bool>irq0,irq1;

    sc_uint<8>CTRL;
    sc_uint<8>TimerValue;
    sc_uint<8>TimerCompare;
    sc_uint<8>TimerIRQStatus;

    bool CTRL_EN;
    bool CTRL_CMP;
    bool CTRL_OV;

    SC_CTOR(Target):timer_socket("Socket"){
        timer_socket.register_b_transport(this,&Target::b_transport);

        SC_THREAD(update_timer);
        sensitive<<clock<<reset;

    }

    void update_timer(){
        while(1){
             wait(clock.read());
                         if(reset.read()){
                cout<<"Im here "<<endl;
                CTRL=0;
                TimerCompare=0;
                TimerValue=0;
                TimerIRQStatus=0;

                irq0.write(0);
                irq1.write(0);
            }
            else {
                           cout<<"Im here also"<<endl; 

                CTRL_EN=CTRL[0];
                cout<<"Enable bit "<<CTRL_EN<<endl;
                if(CTRL_EN){
                    TimerValue++;
                  
                    CTRL_CMP=CTRL[1];
                    if(CTRL_CMP && (TimerValue==TimerCompare)){
                        TimerIRQStatus[0]=1;
                        irq0.write(true);
                        TimerValue=0;
                    }
                    else    
                        irq0.write(0);
                    CTRL_OV=CTRL[2];
                    if(CTRL_OV && (TimerValue==0xFF)){
                        TimerIRQStatus[1]=1;
                        irq1.write(1);
                        TimerValue=0;
                    }
                    else
                        irq1.write(0);
                }
            }
        }
    }
        virtual void b_transport(tlm_generic_payload &trans,sc_time &delay){
            tlm_command cmd=trans.get_command();
            uint64_t addr=trans.get_address();
            unsigned char *ptr=trans.get_data_ptr();
            unsigned int len=trans.get_data_length();
            unsigned char *byt =trans.get_byte_enable_ptr();
            unsigned int wid =trans.get_streaming_width();

            if(addr>=0x10 || byt!=0 || wid<len || len>4){
                trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
                return;
            }
            sc_uint<8>data;
            memcpy(&data,ptr,len);
            if(cmd==TLM_WRITE_COMMAND){
               switch(addr){
                case 0x00:
                    CTRL=data;
                    break;
                case 0x08:
                    TimerCompare=data;
                    break;
                case 0x0c:
                    TimerIRQStatus &=~data;
                    break;
                default:
                    break;

               }
            }
            else if(cmd==TLM_READ_COMMAND){
                 switch(addr){
                case 0x00:
                    data=CTRL;
                    break;
                case 0x04:
                    data=TimerValue;
                    break;
                case 0x08:
                    data=TimerCompare;
                    break;
                case 0x0c:
                    data=TimerIRQStatus ;
                    break;
                default:
                    break;
            }
            memcpy(ptr,&data,len);

        }
        trans.set_response_status(TLM_OK_RESPONSE);
        }

};


#endif