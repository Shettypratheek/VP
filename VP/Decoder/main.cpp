#include<systemc.h>


SC_MODULE(Decoder){
    sc_in<bool>A0,A1;
    sc_in<bool>en;
    sc_out<bool>Y0;
    sc_out<bool>Y1;
    sc_out<bool>Y2;
    sc_out<bool>Y3;
    void decode(){
        sc_uint<4>out=0;
        if(en.read())
            out[((int)A1.read())<<1 | A0.read()]=1;
        Y0.write(out[0]);
        Y1.write(out[1]);
        Y2.write(out[2]);
        Y3.write(out[3]);
    }
    SC_CTOR(Decoder){
        SC_METHOD(decode);
        sensitive<<A0<<A1<<en;

    }
};

SC_MODULE(TB_Decoder){
    sc_out<bool>A0,A1;
    sc_out<bool>en;
    sc_in<bool>Y0;
    sc_in<bool>Y1;
    sc_in<bool>Y2;
    sc_in<bool>Y3;

    void test(){
        struct inputOutputMap{
            int en;
            int A1;
            int A0;
            int Y3;
            int Y2;
            int Y1;
            int Y0;
        };
        struct inputOutputMap test_in_out[8]={
            {0,0,0, 0,0,0,0},
            {0,0,1, 0,0,0,0},
            {0,1,0, 0,0,0,0},
            {0,1,1, 0,0,0,0},
            {1,0,0, 0,0,0,1},
            {1,0,1, 0,0,1,0},
            {1,1,0, 0,1,0,0},
            {1,1,1, 1,0,0,0},
        };
        for(int count=0;count<8;count++){
            A0.write(test_in_out[count].A0);
            A1.write(test_in_out[count].A1);
            en.write(test_in_out[count].en);
            wait(2,SC_NS);
            cout<<"Iteration: "<<count+1<<"\n"<<endl;
            
            cout<<en<<" "<<A1<<" "<<A0<<" "<<Y3<<" "<<Y2<<" "<<Y1<<" "<<Y0<<endl;
        }
        sc_stop();
    }
    SC_CTOR(TB_Decoder){
            SC_THREAD(test);
    }
};

int sc_main(int argc,char *argv[]){
    sc_signal<bool>A0,A1,en,Y0,Y1,Y2,Y3;

    Decoder dut("Decoder");
    dut.A0(A0);
    dut.A1(A1);
    dut.en(en);
    dut.Y0(Y0);
    dut.Y1(Y1);
    dut.Y2(Y2);
    dut.Y3(Y3);

    TB_Decoder tes_mod("Test_Bench");
    tes_mod.A0(A0);
    tes_mod.A1(A1);
    tes_mod.en(en);
    tes_mod.Y0(Y0);
    tes_mod.Y1(Y1);
    tes_mod.Y2(Y2);
    tes_mod.Y3(Y3);

    sc_start();
    return 0;
}