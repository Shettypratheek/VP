#ifndef _TIMER_COUNTER_H_
#define _TIMER_COUNTER_H_

#include <systemc.h>
SC_MODULE(TimerCounter){
    sc_in<sc_time>clock;
    sc_in<bool>reset;
    sc_in<sc_uint<8>>address;
    sc_in<sc_uint<8>>data_in;
    sc_out<sc_uint<8>>data_out;
    sc_in<bool>write_en;
    sc_in<bool>read_en;

    sc_out<bool>irq0,irq1;


    sc_uint<8>CTRL;
    sc_uint<8>TimerValue;
    sc_uint<8>TimerCompare;
    sc_uint<8>TimerIRQStatus;

    bool CTRL_EN;
    bool CTRL_CMP;
    bool CTRL_OV;

    void update_timer(){
        while(true){
            wait(10,SC_NS);
            if(reset.read()){
                CTRL=0;
                TimerCompare=0;
                TimerValue=0;
                TimerIRQStatus=0;
                irq0.write(false);
                irq1.write(false);
            }
            else{
                if(write_en.read()){
                    switch(address.read()){
                        case 0x00:
                            CTRL=data_in.read();
                            break;
                        case 0x08:
                            TimerCompare=data_in.read();
                            break;
                        case 0x0C:
                            TimerIRQStatus &= ~data_in.read();
                            break;
                        default:
                            break;
                    }
                }
                if(read_en.read()){
                    switch(address.read()){
                        case 0x00:
                            data_out.write(CTRL);
                            break;
                        case 0x04:
                            data_out.write(TimerValue);
                            break;
                        case 0x08:
                            data_out.write(TimerCompare);
                            break;
                        case 0x0C:
                            data_out.write(TimerIRQStatus);
                            break;
                        default:
                            break;
                    }
                }

                CTRL_EN=CTRL[0];
                if(CTRL_EN){
                    TimerValue++;

                    CTRL_CMP=CTRL[1];
                    if(CTRL_CMP && (TimerValue==TimerCompare)){
                        TimerIRQStatus[0]=1;
                        irq0.write(true);
                        TimerValue=0;
                    }
                    else   
                        irq0.write(false);
                    CTRL_OV=CTRL[2];
                    if(CTRL &&(TimerValue==0xFF)){
                        TimerIRQStatus[1]=1;
                        irq1.write(true);
                        TimerValue=0;
                    }
                    else   
                        irq1.write(false);
                }
            }
            
        }
    }
    SC_CTOR(TimerCounter){
        SC_THREAD(update_timer);
        sensitive<<clock;
    }
};


#endif