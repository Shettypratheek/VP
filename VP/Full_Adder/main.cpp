#include <systemc.h>

SC_MODULE(full_adder){
    sc_in<bool>in1,in2,in3;
    sc_out<bool>sum,co;

    void compute(){
        sum.write(in1.read()^in2.read() ^ in3.read());
        co.write((in1.read()&in2.read())|(in2.read() & in3.read())|(in3.read() & in1.read()));
    }
    SC_CTOR(full_adder){
        SC_METHOD(compute);
        sensitive<<in1<<in2<<in3;
    }
};

SC_MODULE(TB_full_adder){
    sc_out<bool>in1,in2,in3;
    sc_in<bool>sum,co;

    void test(){
        struct inputs{
            int inA,inB,inC,su,ca;
        };
        struct inputs test_input[8]={
            {0,0,0,0,0},
            {0,0,1,1,0},
            {0,1,0,1,0},
            {0,1,1,0,1},
            {1,0,0,1,0},
            {1,0,1,0,1},
            {1,1,0,0,1},
            {1,1,1,1,1},
        };
        for(int count=0;count<8;count++){
            in1.write(test_input[count].inA);
            in2.write(test_input[count].inB);
            in3.write(test_input[count].inC);
            wait(1,SC_NS);
            cout<<"Iteration: "<<count+1<<endl;
            cout<<"  "<<in1.read()<<" "<<in2.read()<<" "<<in3.read()<<" "<<co.read()<<" "<<sum.read()<<endl;
            assert(sum.read()==test_input[count].su);
            assert(co.read()==test_input[count].ca);
        }
        sc_stop();
    }
    SC_CTOR(TB_full_adder){
        SC_THREAD(test);
}
};

int sc_main(int argc,char *argv[]){
    sc_signal<bool>in1,in2,in3,sum,co;

    full_adder dut("full_adder");
    dut.in1(in1);
    dut.in2(in2);
    dut.in3(in3);
    dut.sum(sum);
    dut.co(co);

    TB_full_adder test_mod("Test_bench");
    test_mod.in1(in1);
    test_mod.in2(in2);
    test_mod.in3(in3);
    test_mod.sum(sum);
    test_mod.co(co);

    sc_start();
    return 0;
}