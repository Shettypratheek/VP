#include <systemc.h>

SC_MODULE(Timercounter){
    sc_in<bool>clk;
    sc_in<bool>rst;

    sc_out<sc_uint<8>>out;

    sc_uint<8>count;

    void compute(){
        if(rst.read()){
            count=0;
           
        }
        else if(clk.posedge())
        {
            count=count+1;
            
        }
        out.write(count);
    }
    SC_CTOR(Timercounter){
        SC_METHOD(compute);
        sensitive<<clk.pos();
        sensitive<<rst;
    }
};

SC_MODULE(tb_counter){
    sc_signal<bool>clk;
    sc_signal<bool>rst;
    sc_signal<sc_uint<8>>count;

  

    Timercounter *timercounter;

    void clk_gen(){
        while(true){
        clk.write(true);
        wait(5,SC_NS);
        clk.write(false);
        wait(5,SC_NS);
        }
    }

    void test(){
        rst.write(true);
        wait(5,SC_NS);
        rst.write(false);
        wait(5,SC_NS);

        for(int i=0;i<300;i++){
            wait(5,SC_NS);
            cout<<sc_time_stamp()<<"|Count:"<<count.read()<<endl;
           
        }
        sc_stop();
    }

    SC_CTOR(tb_counter){
        timercounter=new Timercounter("timer_counter");
        timercounter->clk(clk);
        timercounter->rst(rst);
        timercounter->out(count);

        SC_THREAD(clk_gen);
        SC_THREAD(test);
        sc_trace_file *tf = sc_create_vcd_trace_file("trace");
         sc_trace(tf, clk, "clk");
        sc_trace(tf, rst, "rst");
        sc_trace(tf, count, "count");
    }


};
int sc_main(int argc,char*argv[]){
    tb_counter tb("Counter");

    sc_start();

    return 0;
}