#include <systemc.h>

SC_MODULE(Memory){
    sc_in<sc_uint<32>>address;
    sc_in<bool>read_or_write_en;
    sc_in<sc_uint<32>>data_to_mem;
    sc_out<sc_uint<32>>data_from_mem;

    static const int MEMORY_SIZE=1024;

    unsigned int *mem;
    bool initialised;
    SC_CTOR(Memory){
        mem=new unsigned int [MEMORY_SIZE];

        for(int i=0;i<MEMORY_SIZE;i++)
            mem[i]=0;
        initialised=false;
        SC_METHOD(memoryoperation);
        sensitive <<read_or_write_en<<address;
    }
    ~Memory(){
        delete[] mem;
    }
    void memoryoperation(){
        if(!initialised){
            initialised=true;
            return;
        }
        if(read_or_write_en.read()==0){
            
          
              data_from_mem=mem[address.read()]+1;
            
            cout<<"Read data "<<data_from_mem.read()<<" from address "<<address.read()<<endl;
             
        }
        else{
            cout<<" Data to be written is "<<data_to_mem.read()<<" to memory address "<<address.read()<<endl;
            mem[address.read()]=data_to_mem.read();
        }
    }
};

SC_MODULE(TB_Memory){
    sc_out<sc_uint<32>>address;
    sc_out<bool>read_or_write_en;
    sc_out<sc_uint<32>>data_to_mem;
    sc_in<sc_uint<32>>data_from_mem;

    static const int MEMORY_SIZE=8;

    unsigned int buff1[MEMORY_SIZE];
    unsigned int buff2[MEMORY_SIZE];

    void test(){
        for(int i=0;i<MEMORY_SIZE;i++)
            buff1[i]=i;
        for(int i=0;i<MEMORY_SIZE;i++)
        {
            read_or_write_en.write(true);
            address.write(i);
            data_to_mem.write(buff1[i]);
            wait(1,SC_NS);
        }
       
        for(int i=0;i<MEMORY_SIZE;i++)
        {
            read_or_write_en.write(false);
            address.write(i);
            buff2[i]=data_from_mem.read();
           wait(1,SC_NS);
        }
        bool match=true;
        for(int i=0;i<MEMORY_SIZE;i++){
            if(buff1[i]!=buff2[i]){
                match=false;
                break;
            }
        }
        if(match)
            cout<<" Memory content matched "<<endl;
        else
            cout<<" Memory content does not match"<<endl;

        sc_stop();
    }
    SC_CTOR(TB_Memory){
        SC_THREAD(test);
    }

};

int sc_main(int argc,char *argv[]){
    sc_signal<sc_uint<32>>address;
    sc_signal<bool>read_or_write_en;
    sc_signal<sc_uint<32>>data_to_mem,data_from_mem;

    Memory mem("Memory");
    mem.address(address);
    mem.read_or_write_en(read_or_write_en);
    mem.data_from_mem(data_from_mem);
    mem.data_to_mem(data_to_mem);

    TB_Memory tb_mem("test_bench");
    tb_mem.address(address);
    tb_mem.read_or_write_en(read_or_write_en);
    tb_mem.data_from_mem(data_from_mem);
    tb_mem.data_to_mem(data_to_mem);

    sc_start();
    return 0;
}