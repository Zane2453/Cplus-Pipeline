#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

int general(int cc);                                    //function that generate ALUout and the others

int r0=0,r1=9,r2=8,r3=7,r4=1,r5=2,r6=3,r7=4,r8=5,r9=6;  //register value
int memory[20];                                         //memory value
int count=0,cycle=0,count1;                             //count is the number of instruction;
                                                        //cycle is the number of cycle;
                                                        //count1 is the times to execute general
int pc=1,position=0,position_2=0,position_3=0,position_5=0,ctrl=0,rs[50],rt[50],rd[50],sour[50],targ[50],ext[50],dest[50];
                                                        //position is in the outer loop
                                                        //position_2 is in MEM\WB stage
                                                        //position_3 is in EXE\MEM stage
int ALUout[50],Rdata[50];           //ALUoutput, ReadData
int dcount=0,ecount=0,mcount=0;     //doing each stage's count
char control[500];                  //control signal
char ins[500];                      //instruction's array
int position_4=0,count2=0;          //when write back's register
int isLoad=0,bubble=0,count3,ALUbubble;   //this is to detect whether load hazard happen and to solve it

int main()
{
    memory[0]=5;
    memory[4]=9;
    memory[8]=4;
    memory[12]=8;
    memory[16]=7;
    for(int i=0;i<500;i++){
        ins[i]=0;
    }
    char file[]="Lwhazard.txt";
    ifstream fin(file);         //read the .txt file
    for(int i=0;i<500;i++){
        fin>>ins[i];
    }
    fin.close();

    for(int i=0;i<500;i++){     //calculate the instruction count
        if(ins[i]==0){
            count=i;
            break;
        }
    }

    while(1){
        //determine rs,rt,rd and ext
        rs[cycle]=(ins[position+6]-48)*2*2*2*2+(ins[position+7]-48)*2*2*2+(ins[position+8]-48)*2*2+(ins[position+9]-48)*2+(ins[position+10]-48);
        rt[cycle]=(ins[position+11]-48)*2*2*2*2+(ins[position+12]-48)*2*2*2+(ins[position+13]-48)*2*2+(ins[position+14]-48)*2+(ins[position+15]-48);
        rd[cycle]=(ins[position+16]-48)*2*2*2*2+(ins[position+17]-48)*2*2*2+(ins[position+18]-48)*2*2+(ins[position+19]-48)*2+(ins[position+20]-48);
        ext[cycle]=(ins[position+16]-48)*pow(2,15)+(ins[position+17]-48)*pow(2,14)+(ins[position+18]-48)*pow(2,13)+(ins[position+19]-48)*pow(2,12)+(ins[position+20]-48)*pow(2,11)+(ins[position+21]-48)*pow(2,10)+(ins[position+22]-48)*pow(2,9)+(ins[position+23]-48)*pow(2,8)+(ins[position+24]-48)*pow(2,7)+(ins[position+25]-48)*pow(2,6)+(ins[position+26]-48)*pow(2,5)+(ins[position+27]-48)*pow(2,4)+(ins[position+28]-48)*pow(2,3)+(ins[position+29]-48)*pow(2,2)+(ins[position+30]-48)*pow(2,1)+(ins[position+31]-48)*pow(2,0);

        //determine the control signal
        //R_type
        if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'){
            control[ctrl]='1';
            control[ctrl+1]='1';
            control[ctrl+2]='0';
            control[ctrl+3]='0';
            control[ctrl+4]='0';
            control[ctrl+5]='0';
            control[ctrl+6]='0';
            control[ctrl+7]='1';
            control[ctrl+8]='0';
        }
        //Addi instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'){
            control[ctrl]='0';
            control[ctrl+1]='0';
            control[ctrl+2]='0';
            control[ctrl+3]='1';
            control[ctrl+4]='0';
            control[ctrl+5]='0';
            control[ctrl+6]='0';
            control[ctrl+7]='1';
            control[ctrl+8]='0';
        }
        //Andi instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='1'&&ins[position+4]=='0'&&ins[position+5]=='0'){
            control[ctrl]='0';
            control[ctrl+1]='1';
            control[ctrl+2]='1';
            control[ctrl+3]='1';
            control[ctrl+4]='0';
            control[ctrl+5]='0';
            control[ctrl+6]='0';
            control[ctrl+7]='1';
            control[ctrl+8]='0';
        }
        //Load
        else if(ins[position]=='1'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='1'&&ins[position+5]=='1'){
            control[ctrl]='0';
            control[ctrl+1]='0';
            control[ctrl+2]='0';
            control[ctrl+3]='1';
            control[ctrl+4]='0';
            control[ctrl+5]='1';
            control[ctrl+6]='0';
            control[ctrl+7]='1';
            control[ctrl+8]='1';
        }
        //Store
        else if(ins[position]=='1'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='0'&&ins[position+4]=='1'&&ins[position+5]=='1'){
            control[ctrl]='0';
            control[ctrl+1]='0';
            control[ctrl+2]='0';
            control[ctrl+3]='1';
            control[ctrl+4]='0';
            control[ctrl+5]='0';
            control[ctrl+6]='1';
            control[ctrl+7]='0';
            control[ctrl+8]='0';
        }
        //Branch not equal
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='1'&&ins[position+4]=='0'&&ins[position+5]=='1'){
            control[ctrl]='0';
            control[ctrl+1]='0';
            control[ctrl+2]='1';
            control[ctrl+3]='0';
            control[ctrl+4]='1';
            control[ctrl+5]='0';
            control[ctrl+6]='0';
            control[ctrl+7]='0';
            control[ctrl+8]='0';
        }

        position=position+32;
        ctrl=ctrl+9;
        cycle++;
        if(ins[position]==0)
            break;
    }

    //set the parameter and do function general
    position=0;
    ctrl=0;
    count1=count/32;
    general(0);

    //reset the parameter
    position=0;
    ctrl=0;
    count1=count/32+5-1;
    count=count/32;
    count3=count;
    cycle=1;

    ofstream fout("loadResult.txt");
    while(1){
        fout<<"CC "<<cycle<<"\n\n";
        fout<<"Registers:\n";
        fout<<"$0: "<<r0<<endl<<"$1: "<<r1<<endl<<"$2: "<<r2<<endl<<"$3: "<<r3<<endl<<"$4: "<<r4<<endl<<"$5: "<<r5<<endl<<"$6: "<<r6<<endl<<"$7: "<<r7<<endl<<"$8: "<<r8<<endl<<"$9: "<<r9<<"\n\n";
        fout<<"Data memory:\n";
        fout<<"0x00: "<<memory[0]<<endl<<"0x04: "<<memory[4]<<endl<<"0x08: "<<memory[8]<<endl<<"0x0C: "<<memory[12]<<endl<<"0x10: "<<memory[16]<<"\n\n";

        //IF/ID Stage
        fout<<"IF/ID :\n";
        fout<<"PC\t\t"<<pc*4<<endl;
        fout<<"Instruction\t";
        if(ins[position]!=0){
            for(int i=0;i<32;i++)
                fout<<ins[position+i];
        }
        else{
            fout<<"00000000000000000000000000000000";
        }
        fout<<"\n\n";

        //ID/EXE Stage
        fout<<"ID/EX :\n";
        if(cycle>1&&cycle<2+count3){
            fout<<"ReadData1\t"<<sour[dcount]<<endl<<"ReadData2\t"<<targ[dcount]<<endl<<"sign_ext\t"<<ext[dcount]<<endl;
            fout<<"Rs\t\t"<<rs[dcount]<<endl<<"Rt\t\t"<<rt[dcount]<<endl<<"Rd\t\t"<<rd[dcount]<<endl<<"Control signals\t";

            //this is a bubble in this stage
            if(isLoad==1&&bubble==1){
                fout<<"000000000";
            }
            else{
                for(int i=dcount*9;i<dcount*9+9;i++)
                    fout<<control[i];
                    dcount++;
                    position_5=position_5+32;
            }
        }
        else {
            fout<<"ReadData1\t0"<<endl<<"ReadData2\t0"<<endl<<"sign_ext\t0"<<endl;
            fout<<"Rs\t\t0"<<endl<<"Rt\t\t0"<<endl<<"Rd\t\t0"<<endl<<"Control signals\t000000000";
        }
        fout<<endl<<endl;

        //EXE/MEM Stage
        fout<<"EX/MEM :\n";
        if(cycle>2&&cycle<3+count3){

            //position_3 is the position in EXE\MEM register
            //Store
            if(ins[position_3]=='1'&&ins[position_3+1]=='0'&&ins[position_3+2]=='1'&&ins[position_3+3]=='0'&&ins[position_3+4]=='1'&&ins[position_3+5]=='1'){
                if(rt[ecount]==0)       //store the value to memory if the instruction is 'store'
                    memory[ALUout[ecount]]=r0;
                else if(rt[ecount]==1)
                    memory[ALUout[ecount]]=r1;
                else if(rt[ecount]==2)
                    memory[ALUout[ecount]]=r2;
                else if(rt[ecount]==3)
                    memory[ALUout[ecount]]=r3;
                else if(rt[ecount]==4)
                    memory[ALUout[ecount]]=r4;
                else if(rt[ecount]==5)
                    memory[ALUout[ecount]]=r5;
                else if(rt[ecount]==6)
                    memory[ALUout[ecount]]=r6;
                else if(rt[ecount]==7)
                    memory[ALUout[ecount]]=r7;
                else if(rt[ecount]==8)
                    memory[ALUout[ecount]]=r8;
                else if(rt[ecount]==9)
                    memory[ALUout[ecount]]=r9;
            }

            //because the value in memory change, need to reset the ALUout and the others
            position_4=position;
            count2=count1;
            position=0;
            count1=count;
            general(0);
            ctrl=0;
            position=position_4;
            count1=count2;

            //this is a bubble in this stage
            if(isLoad==1&&bubble==2){

                //decide the ALUout in the bubble stage
                if(ins[position_5-64]=='1'&&ins[position_5-63]=='0'&&ins[position_5-62]=='0'&&ins[position_5-61]=='0'&&ins[position_5-60]=='1'&&ins[position_5-59]=='1'){
                    if(ecount>=2&&control[ctrl-11]=='1'&&rd[ecount-2]!=0){
                        if(rt[ecount]==rd[ecount-2])
                            ALUbubble=ALUout[ecount-2]+ALUout[ecount-1];
                        else
                            ALUbubble=ALUout[ecount-1]+targ[ecount];
                    }
                    if(ecount>=1&&control[ctrl-2]=='1'&&rd[ecount-1]!=0){
                        if(rt[ecount]==rd[ecount-1])
                            ALUbubble=ALUout[ecount-1]+ALUout[ecount-1];
                        else
                            ALUbubble=ALUout[ecount-1]+targ[ecount];
                    }
                    else
                        ALUbubble=ALUout[ecount-1]+targ[ecount];
                }
                //ALUout is "ALUbubble", Rt/Rd is "rt"
                fout<<"ALUout\t\t"<<ALUbubble<<endl<<"WriteData\t"<<targ[ecount]<<endl<<"Rt/Rd\t\t"<<rt[ecount]<<endl<<"Control signals\t00000";
            }
            else{
                fout<<"ALUout\t\t"<<ALUout[ecount]<<endl<<"WriteData\t"<<targ[ecount]<<endl<<"Rt/Rd\t\t"<<dest[ecount]<<endl<<"Control signals\t";
                for(int i=ecount*9+4;i<ecount*9+9;i++)
                    fout<<control[i];
                ecount++;
                position_3=position_3+32;
            }
        }
        else {
            fout<<"ALUout\t\t0"<<endl<<"WriteData\t0"<<endl<<"Rt/Rd\t\t0"<<endl<<"Control signals\t000000";
        }
        fout<<endl<<endl;

        //MEM/WB Stage
        fout<<"MEM/WB :\n";
        if(cycle>3&&cycle<4+count3){
            //this is a bubble in this stage
            if(isLoad==1&&bubble==3)
                fout<<"ReadData\t"<<Rdata[mcount]<<endl<<"ALUout\t\t"<<ALUbubble<<endl<<"Rt/Rd\t\t"<<rt[mcount]<<endl<<"Control signals\t00";
            else{
                fout<<"ReadData\t"<<Rdata[mcount]<<endl<<"ALUout\t\t"<<ALUout[mcount]<<endl<<"Rt/Rd\t\t"<<dest[mcount]<<endl<<"Control signals\t";
                for(int i=mcount*9+7;i<mcount*9+9;i++)
                    fout<<control[i];

                //position_2 is the position in MEM/WB register
                //R_type
                if(ins[position_2]=='0'&&ins[position_2+1]=='0'&&ins[position_2+2]=='0'&&ins[position_2+3]=='0'&&ins[position_2+4]=='0'&&ins[position_2+5]=='0'){
                    if(rd[mcount]==0)       //store the value to register if the instruction is 'R_type'
                        r0=ALUout[mcount];
                    else if(rd[mcount]==1)
                        r1=ALUout[mcount];
                    else if(rd[mcount]==2)
                        r2=ALUout[mcount];
                    else if(rd[mcount]==3)
                        r3=ALUout[mcount];
                    else if(rd[mcount]==4)
                        r4=ALUout[mcount];
                    else if(rd[mcount]==5)
                        r5=ALUout[mcount];
                    else if(rd[mcount]==6)
                        r6=ALUout[mcount];
                    else if(rd[mcount]==7)
                        r7=ALUout[mcount];
                    else if(rd[mcount]==8)
                        r8=ALUout[mcount];
                    else if(rd[mcount]==9)
                        r9=ALUout[mcount];
                }
                //Addi instruction
                else if(ins[position_2]=='0'&&ins[position_2+1]=='0'&&ins[position_2+2]=='1'&&ins[position_2+3]=='0'&&ins[position_2+4]=='0'&&ins[position_2+5]=='0'){
                    if(rt[mcount]==0)       //take the value to register if the instruction is 'addi'
                        r0=ALUout[mcount];
                    else if(rt[mcount]==1)
                        r1=ALUout[mcount];
                    else if(rt[mcount]==2)
                        r2=ALUout[mcount];
                    else if(rt[mcount]==3)
                        r3=ALUout[mcount];
                    else if(rt[mcount]==4)
                        r4=ALUout[mcount];
                    else if(rt[mcount]==5)
                        r5=ALUout[mcount];
                    else if(rt[mcount]==6)
                        r6=ALUout[mcount];
                    else if(rt[mcount]==7)
                        r7=ALUout[mcount];
                    else if(rt[mcount]==8)
                        r8=ALUout[mcount];
                    else if(rt[mcount]==9)
                        r9=ALUout[mcount];
                }
                //Andi instruction
                else if(ins[position_2]=='0'&&ins[position_2+1]=='0'&&ins[position_2+2]=='1'&&ins[position_2+3]=='1'&&ins[position_2+4]=='0'&&ins[position_2+5]=='0'){
                    if(rt[mcount]==0)       //take the value to register if the instruction is 'andi'
                        r0=ALUout[mcount];
                    else if(rt[mcount]==1)
                        r1=ALUout[mcount];
                    else if(rt[mcount]==2)
                        r2=ALUout[mcount];
                    else if(rt[mcount]==3)
                        r3=ALUout[mcount];
                    else if(rt[mcount]==4)
                        r4=ALUout[mcount];
                    else if(rt[mcount]==5)
                        r5=ALUout[mcount];
                    else if(rt[mcount]==6)
                        r6=ALUout[mcount];
                    else if(rt[mcount]==7)
                        r7=ALUout[mcount];
                    else if(rt[mcount]==8)
                        r8=ALUout[mcount];
                    else if(rt[mcount]==9)
                        r9=ALUout[mcount];
                }
                //Load
                else if(ins[position_2]=='1'&&ins[position_2+1]=='0'&&ins[position_2+2]=='0'&&ins[position_2+3]=='0'&&ins[position_2+4]=='1'&&ins[position_2+5]=='1'){
                    if(rt[mcount]==0)       //take the value from memory to register if the instruction is 'load'
                        r0=memory[ALUout[mcount]];
                    else if(rt[mcount]==1)
                        r1=memory[ALUout[mcount]];
                    else if(rt[mcount]==2)
                        r2=memory[ALUout[mcount]];
                    else if(rt[mcount]==3)
                        r3=memory[ALUout[mcount]];
                    else if(rt[mcount]==4)
                        r4=memory[ALUout[mcount]];
                    else if(rt[mcount]==5)
                        r5=memory[ALUout[mcount]];
                    else if(rt[mcount]==6)
                        r6=memory[ALUout[mcount]];
                    else if(rt[mcount]==7)
                        r7=memory[ALUout[mcount]];
                    else if(rt[mcount]==8)
                        r8=memory[ALUout[mcount]];
                    else if(rt[mcount]==9)
                        r9=memory[ALUout[mcount]];
                }

                //because the value in register change, need to reset the ALUout and the others
                position_4=position;
                count2=count1;
                position=0;
                count1=count;
                general(0);
                ctrl=0;
                position=position_4;
                count1=count2;

                mcount++;
                position_2=position_2+32;
            }
        }
        else {
            fout<<"ReadData\t0"<<endl<<"ALUout\t\t0"<<endl<<"Rt/Rd\t\t0"<<endl<<"Control signals\t00";
        }
        fout<<endl<<"================================================================="<<endl;

        position=position+32;
        pc++;
        count1--;

        //load hazard detect
        if(ins[position_5-32]=='1'&&ins[position_5-31]=='0'&&ins[position_5-30]=='0'&&ins[position_5-29]=='0'&&ins[position_5-28]=='1'&&ins[position_5-27]=='1'&&(rt[dcount-1]==rs[dcount]||rt[dcount-1]==rt[dcount])){
            isLoad=1;
        }
        //the movement to stall different stage in different cycle
        if(isLoad==1){
            bubble++;
            //the adjustment if load hazard happen
            if(bubble==1){
                position=position-32;
                pc--;
                count1++;
                count3++;
            }
            //the bubble is in the end
            else if(bubble==4)
                isLoad==0;
        }
        cycle++;
        if(count1==0){
            break;
        }
    }
    fout.close();
    return 0;
}

int general(int cc){
    while(1){
        if(rs[cc]==0)       //determine source's value
            sour[cc]=r0;
        else if(rs[cc]==1)
            sour[cc]=r1;
        else if(rs[cc]==2)
            sour[cc]=r2;
        else if(rs[cc]==3)
            sour[cc]=r3;
        else if(rs[cc]==4)
            sour[cc]=r4;
        else if(rs[cc]==5)
            sour[cc]=r5;
        else if(rs[cc]==6)
            sour[cc]=r6;
        else if(rs[cc]==7)
            sour[cc]=r7;
        else if(rs[cc]==8)
            sour[cc]=r8;
        else if(rs[cc]==9)
            sour[cc]=r9;

        if(rt[cc]==0)       //determine target's value
            targ[cc]=r0;
        else if(rt[cc]==1)
            targ[cc]=r1;
        else if(rt[cc]==2)
            targ[cc]=r2;
        else if(rt[cc]==3)
            targ[cc]=r3;
        else if(rt[cc]==4)
            targ[cc]=r4;
        else if(rt[cc]==5)
            targ[cc]=r5;
        else if(rt[cc]==6)
            targ[cc]=r6;
        else if(rt[cc]==7)
            targ[cc]=r7;
        else if(rt[cc]==8)
            targ[cc]=r8;
        else if(rt[cc]==9)
            targ[cc]=r9;

        //determine the destination
        //determine the ALUout
        //have the forwarding function
        //Add instruction
        if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'&&ins[position+26]=='1'&&ins[position+27]=='0'&&ins[position+28]=='0'&&ins[position+29]=='0'&&ins[position+30]=='0'&&ins[position+31]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]+targ[cc];
                else if(rt[cc]==rd[cc-2])
                    ALUout[cc]=sour[cc]+ALUout[cc-2];
                else
                    ALUout[cc]=sour[cc]+targ[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]+targ[cc];
                else if(rt[cc]==rd[cc-1])
                    ALUout[cc]=sour[cc]+ALUout[cc-1];
                else
                    ALUout[cc]=sour[cc]+targ[cc];
            }
            else
                ALUout[cc]=sour[cc]+targ[cc];
            dest[cc]=rd[cc];
        }
        //Sub instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'&&ins[position+26]=='1'&&ins[position+27]=='0'&&ins[position+28]=='0'&&ins[position+29]=='0'&&ins[position+30]=='1'&&ins[position+31]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]-targ[cc];
                else if(rt[cc]==rd[cc-2])
                    ALUout[cc]=sour[cc]-ALUout[cc-2];
                else
                    ALUout[cc]=sour[cc]-targ[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]-targ[cc];
                else if(rt[cc]==rd[cc-1])
                    ALUout[cc]=sour[cc]-ALUout[cc-1];
                else
                    ALUout[cc]=sour[cc]-targ[cc];
            }
            else
                ALUout[cc]=sour[cc]-targ[cc];
            dest[cc]=rd[cc];
        }
        //And instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'&&ins[position+26]=='1'&&ins[position+27]=='0'&&ins[position+28]=='0'&&ins[position+29]=='1'&&ins[position+30]=='0'&&ins[position+31]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){

                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]&targ[cc];
                else if(rt[cc]==rd[cc-2])
                    ALUout[cc]=sour[cc]&ALUout[cc-2];
                else
                    ALUout[cc]=(sour[cc])&(targ[cc]);
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]&targ[cc];
                else if(rt[cc]==rd[cc-1])
                    ALUout[cc]=sour[cc]&ALUout[cc-1];
                else
                    ALUout[cc]=(sour[cc])&(targ[cc]);
            }
            else
                ALUout[cc]=(sour[cc])&(targ[cc]);
            dest[cc]=rd[cc];
        }
        //Or instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'&&ins[position+26]=='1'&&ins[position+27]=='0'&&ins[position+28]=='0'&&ins[position+29]=='1'&&ins[position+30]=='0'&&ins[position+31]=='1'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]|targ[cc];
                else if(rt[cc]==rd[cc-2])
                    ALUout[cc]=sour[cc]|ALUout[cc-2];
                else
                    ALUout[cc]=sour[cc]|targ[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]|targ[cc];
                else if(rt[cc]==rd[cc-1])
                    ALUout[cc]=sour[cc]|ALUout[cc-1];
                else
                    ALUout[cc]=sour[cc]|targ[cc];
            }
            else
                ALUout[cc]=sour[cc]|targ[cc];
            dest[cc]=rd[cc];
        }
        //Set on less than instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'&&ins[position+26]=='1'&&ins[position+27]=='0'&&ins[position+28]=='1'&&ins[position+29]=='0'&&ins[position+30]=='1'&&ins[position+31]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2]){
                    if((ALUout[cc-2])-(targ[cc])<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
                else if(rt[cc]==rd[cc-2]){
                    if((sour[cc])-((ALUout[cc-2]))<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
                else{
                    if((sour[cc])-(targ[cc])<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1]){
                    if((ALUout[cc-1])-(targ[cc])<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
                else if(rt[cc]==rd[cc-1]){
                    if((sour[cc])-((ALUout[cc-1]))<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
                else{
                    if((sour[cc])-(targ[cc])<0)
                        ALUout[cc]=1;
                    else
                        ALUout[cc]=0;
                }
            }
            else{
                if((sour[cc])-(targ[cc])<0)
                    ALUout[cc]=1;
                else
                    ALUout[cc]=0;
            }
            dest[cc]=rd[cc];
        }
        //Addi instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='0'&&ins[position+4]=='0'&&ins[position+5]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            else
                ALUout[cc]=sour[cc]+ext[cc];
            dest[cc]=rt[cc];
        }
        //Andi instruction
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='1'&&ins[position+4]=='0'&&ins[position+5]=='0'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]&ext[cc];
                else
                    ALUout[cc]=sour[cc]&ext[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]&ext[cc];
                else
                    ALUout[cc]=sour[cc]&ext[cc];
            }
            else
                ALUout[cc]=sour[cc]&ext[cc];
            dest[cc]=rt[cc];
        }
        //Load
        else if(ins[position]=='1'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='1'&&ins[position+5]=='1'){
            if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            else
                ALUout[cc]=sour[cc]+ext[cc];
            dest[cc]=rt[cc];
        }
        //Store
        else if(ins[position]=='1'&&ins[position+1]=='0'&&ins[position+2]=='1'&&ins[position+3]=='0'&&ins[position+4]=='1'&&ins[position+5]=='1'){
           if(cc>=2&&control[ctrl-11]=='1'&&rd[cc-2]!=0){
                if(rs[cc]==rd[cc-2])
                    ALUout[cc]=ALUout[cc-2]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            if(cc>=1&&control[ctrl-2]=='1'&&rd[cc-1]!=0){
                if(rs[cc]==rd[cc-1])
                    ALUout[cc]=ALUout[cc-1]+ext[cc];
                else
                    ALUout[cc]=sour[cc]+ext[cc];
            }
            else
                ALUout[cc]=sour[cc]+ext[cc];
            dest[cc]=rt[cc];
        }
        //Branch not equal
        else if(ins[position]=='0'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='1'&&ins[position+4]=='0'&&ins[position+5]=='1'){
            ALUout[cc]=sour[cc]-targ[cc];
            dest[cc]=rt[cc];
        }

        //determine ReadData's value
        //it is valid when instruction 'load'
        if(ins[position]=='1'&&ins[position+1]=='0'&&ins[position+2]=='0'&&ins[position+3]=='0'&&ins[position+4]=='1'&&ins[position+5]=='1'){
            if(ALUout[cc]==0)
                Rdata[cc]=memory[0];
            else if(ALUout[cc]==4)
                Rdata[cc]=memory[4];
            else if(ALUout[cc]==8)
                Rdata[cc]=memory[8];
            else if(ALUout[cc]==12)
                Rdata[cc]=memory[12];
            else if(ALUout[cc]==16)
                Rdata[cc]=memory[16];
        }
        else
            Rdata[cc]=0;

        position=position+32;
        ctrl=ctrl+9;
        cc++;
        count1--;
        if(count1==0)
            break;
    }
}
