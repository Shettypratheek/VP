#include <systemc.h>

SC_MODULE(NandG){
    sc_in<bool>a,b;
    sc_out<bool>out;

    void compute(){
        out.write(!(a.read() && b.read()));
        
    }
    SC_CTOR(NandG){
        SC_METHOD(compute);
        sensitive <<a<<b;
    }
};

SC_MODULE(TB_Nand){
    sc_out<bool>a,b;
    sc_in<bool>out;

    void test(){
        a.write(false);
        b.write(false);
        wait(2,SC_NS);
        //assert(out.read()==0);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;

        a.write(false);
        b.write(true);
        wait(2,SC_NS);
        //assert(out.read()==0);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;

        a.write(true);
        b.write(false);             
        wait(2,SC_NS);
        //assert(out.read()==0);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;
        
        a.write(true);
        b.write(true);
        wait(2,SC_NS);
        //assert(out.read()==1);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;
        sc_stop();
    }

    SC_CTOR(TB_Nand){
        SC_THREAD(test);
    }

};

int sc_main(int argc,char *argv[]){
    sc_signal<bool>a,b,out;
    sc_trace_file *Tf=sc_create_vcd_trace_file("traces");
    sc_trace(Tf,a,"a");
    sc_trace(Tf,b,"b");
    sc_trace(Tf,out,"out");
    NandG dut("andGate");
    dut.a(a);
    dut.b(b);
    dut.out(out);

    TB_Nand tb("TB_And");
    tb.a(a);
    tb.b(b);
    tb.out(out);
    sc_start();
    sc_close_vcd_trace_file(Tf);
    return 0;
}