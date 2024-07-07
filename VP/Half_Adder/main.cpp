#include <systemc.h>

SC_MODULE(Half_adder){
    sc_in<bool>inA,inB;
    sc_out<bool>sum,co;

    void compute(){
        sum.write(inA.read()^inB.read());
        co.write(inA.read() && inB.read());
    }
    SC_CTOR(Half_adder){
        SC_METHOD(compute);
        sensitive<<inA<<inB;
    }
};

SC_MODULE(TB_half_adder){
    sc_out<bool>inA,inB;
    sc_in<bool>sum,co;

    void test(){
        struct inout{
            int in1;
            int in2;
            int sum;
            int co;
        };
        struct inout testinput[4]={
            {0,0,0,0},
            {0,1,1,0},
            {1,0,1,0},
            {1,1,0,1},
        };

        for(int count=0;count<4;count++){
            inA.write(testinput[count].in1);
            inB.write(testinput[count].in2);
            wait(2,SC_NS);
            cout<<sc_time_stamp()<<" IN1: "<<inA.read()<<" IN2: "<<inB.read()<<" Carry: "<<co.read()<<" Sum:"<<sum.read()<<endl;
            assert(sum.read()==testinput[count].sum);
            assert(co.read()==testinput[count].co);
        }
        sc_stop();
    }
    SC_CTOR(TB_half_adder){
        SC_THREAD(test);
    }

};


int sc_main(int argc,char *argv[]){
        sc_signal<bool>inA,inB,sum,co;

        Half_adder dut("Half_adder");
        dut.inA(inA);
        dut.inB(inB);
        dut.sum (sum);
        dut.co(co);

        TB_half_adder test_model("Test_Bench");
        test_model.inA(inA);
        test_model.inB(inB);
        test_model.sum(sum);
        test_model.co(co);

        sc_start();
    return 0;
}