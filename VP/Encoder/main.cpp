#include <systemc.h>

SC_MODULE(Encoder){
    sc_in<bool>Y0;
    sc_in<bool>Y1;
    sc_in<bool>Y2;
    sc_in<bool>Y3;
    sc_in<bool>en;
    sc_out<bool>A0;
    sc_out<bool>A1;

    void encode(){
        if(en.read()){
            A0.write(Y3.read() || Y1.read());
            A1.write(Y3.read() || Y2.read());
        }
    }
    SC_CTOR(Encoder){
        SC_METHOD(encode);
        sensitive<<Y0<<Y1<<Y2<<Y3<<en;
    }
};

SC_MODULE(TB_Encoder){
    sc_out<bool>Y0,Y1,Y2,Y3;
    sc_out<bool>en;
    sc_in<bool>A0,A1;

    void test(){
        struct inputOutputMap{
            int en;
            int Y3;
            int Y2;
            int Y1;
            int Y0;
            int A1;
            int A0;
        };
        struct inputOutputMap tes_inout[8]={
            {0,0,0,0,1,0,0},
            {0,0,0,1,0,0,0},
            {0,0,1,0,0,0,0},
            {0,1,0,0,0,0,0},

            {1,0,0,0,1,0,0},
            {1,0,0,1,0,0,1},
            {1,0,1,0,0,1,0},
            {1,1,0,0,0,1,1},
        };
        for(int count=0;count<8;count++){
            Y0.write(tes_inout[count].Y0);
            Y1.write(tes_inout[count].Y1);
            Y2.write(tes_inout[count].Y2);
            Y3.write(tes_inout[count].Y3);
            en.write(tes_inout[count].en);
            wait(2,SC_NS);

            cout<<sc_time_stamp()<<" "<<en<<" "<<Y3<<" "<<Y2<<" "<<Y1<<" "<<Y0<<" "<<A1<<" "<<A0<<"\n"<<endl;
        }
    }
    SC_CTOR(TB_Encoder){
        SC_THREAD(test);
    }
};


int sc_main(int argc,char *argv[]){
    sc_signal<bool>Y3,Y2,Y1,Y0,en,A1,A0;

    Encoder dut("Encoder");
    dut.Y0(Y0);
    dut.Y1(Y1);
    dut.Y2(Y2);
    dut.Y3(Y3);
    dut.en(en);
    dut.A0(A0);
    dut.A1(A1);

    TB_Encoder tes_mod("Testing");
    tes_mod.Y0(Y0);
    tes_mod.Y1(Y1);
    tes_mod.Y2(Y2);
    tes_mod.Y3(Y3);
    tes_mod.en(en);
    tes_mod.A1(A1);
    tes_mod.A0(A0);

    sc_start();
    return 0;
}