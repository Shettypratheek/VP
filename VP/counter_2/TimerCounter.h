#ifndef TIMERCOUNTER_H
#define TIMERCOUNTER_H

#include <systemc.h>
#include <bitset>

SC_MODULE(TimerCounter){
    sc_in<bool>clock;
    sc_in<bool>reset;
    sc_in<sc_uint<8>>address;
    sc_in<sc_uint<8>>data_in;
    sc_out<sc_uint<8>>data_out;

    sc_in<bool>write_en;
    sc_in<bool>read_en;

    sc_out<bool>irq0;
    sc_out<bool>irq1;

    sc_uint<8>TIMER_CONTROL_REG;
    sc_uint<8>TIMER_VALUE_REG;
    sc_uint<8>TIMER_COMPARE_REG;
    sc_uint<8>TIMER_IRQ_STATUS_REG;

    bool TIMER_CONTROL_EN_BIT;
    bool TIMER_CONTROL_CMP_BIT;
    bool TIMER_CONTROL_OV_BIT;

    

    void timer_process(){
        if(reset.read()){
            TIMER_VALUE_REG=0;
            irq0.write(false);
            irq1.write(false);
            TIMER_IRQ_STATUS_REG=0;
        }
        else{
            TIMER_CONTROL_EN_BIT=TIMER_CONTROL_REG[0];

            if(TIMER_CONTROL_EN_BIT){
               
                    TIMER_VALUE_REG=TIMER_VALUE_REG+1;
               
                 
                TIMER_CONTROL_CMP_BIT=TIMER_CONTROL_REG[1];
                if(TIMER_CONTROL_CMP_BIT && TIMER_VALUE_REG==TIMER_COMPARE_REG){
                    irq0.write(true);
                    TIMER_IRQ_STATUS_REG[0]=1;
                    TIMER_VALUE_REG=0;
                    
                }
                else
                    irq0.write(false);

                 TIMER_CONTROL_OV_BIT=TIMER_CONTROL_REG[2];
                if(TIMER_CONTROL_OV_BIT && TIMER_VALUE_REG==0xFF){
                    irq1.write(true);
                    TIMER_IRQ_STATUS_REG[1]=1;
                    TIMER_VALUE_REG=0;
                }
                else
                    irq1.write(false);
                 
            }
        }
    }
    void register_access(){
        if(write_en.read()){
            switch(address.read()){
                case 0x00:
                    TIMER_CONTROL_REG=data_in.read();
                    break;
                case 0x08:
                    TIMER_COMPARE_REG=data_in.read();
                    break;
                case 0x0C:
                    TIMER_IRQ_STATUS_REG &= ~data_in.read();
                    break;
                default:
                    break;
            }
        }
        if(read_en.read()){
            switch(address.read()){
                case 0x00:
                    data_out.write(TIMER_CONTROL_REG);
                    break;
                case 0x04:

                    data_out.write(TIMER_VALUE_REG);
                    
                    break;
                case 0x08:
                    data_out.write(TIMER_COMPARE_REG);
                    break;
                case 0x0C:
                    data_out.write(TIMER_IRQ_STATUS_REG);
                    break;
                default:
                    break;
            }
        }
    }
    SC_CTOR(TimerCounter){
        SC_METHOD(timer_process);
        sensitive<<clock.pos();
        sensitive<<reset;

        SC_METHOD(register_access);
        sensitive<<clock.pos();
    }


};

#endif

