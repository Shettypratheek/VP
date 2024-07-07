#include <systemc.h>

SC_MODULE(NOR){
    sc_in<bool>a,b;
    sc_out<bool>out;

    void compute(){
        out.write(!(a.read() || b.read() ));
    }
    SC_CTOR(NOR){
        SC_METHOD(compute);
        sensitive<<a<<b;
    }
};

SC_MODULE(TB_NOR){
    sc_out<bool>a,b;
    sc_in<bool>out;

    void test(){

        a.write(false);
        b.write(false);
        wait(2,SC_NS);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;

         a.write(false);
        b.write(true);
        wait(2,SC_NS);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;

         a.write(true);
        b.write(false);
        wait(2,SC_NS);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;

         a.write(true);
        b.write(true);
        wait(2,SC_NS);
        cout<<sc_time_stamp()<<" A: "<<a.read()<<" B: "<<b.read()<<" Output: "<<out.read()<<endl;
        sc_stop();
    }
    SC_CTOR(TB_NOR){
        SC_THREAD(test);
    }
};

int sc_main(int argc,char *argv[]){
    sc_signal<bool>a,b,out;

    NOR dut("OR_Gate");
    dut.a(a);
    dut.b(b);
    dut.out(out);

    TB_NOR test("Test Bench");
    test.a(a);
    test.b(b);
    test.out(out);

    sc_start();

    return 0;
}