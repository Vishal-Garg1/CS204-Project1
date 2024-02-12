#include<iostream>
#include<unordered_map>
#include<fstream>
#include<bitset>
using namespace std;

class instructions{
    public:
    unordered_map<string,int> instructionNames;
    bitset<32> instructionIn32bit;
    string instructionInHex;
    bitset<7> opcode;

    instructions(){
        instructionNames.insert(make_pair("add",1));
    }

    void binToHex(){
        instructionInHex="0x";
        for(int i=31;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=instructionIn32bit[i]*j;
                i--;
            }
            if(num<10){
                instructionInHex+=num+'0';
            }else{
                instructionInHex+=num-10+'A';
            }
        }
    }
};

class R:public instructions{
    public:
    bitset<5> rd;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<7> func7;
    
    void fetchInstructionDetails(string line,int i){
        while(line[i]!='x'){
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        int registerValueInDecimal=stoi(registerValue);
        rd=registerValueInDecimal;
        while(line[i]!='x'){
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        registerValueInDecimal=stoi(registerValue);
        rs1=registerValueInDecimal;
        while(line[i]!='x'){
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        registerValueInDecimal=stoi(registerValue);
        rs2=registerValueInDecimal;
        return;
    }

    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<25;idx++){
            instructionIn32bit[idx]=rs2[idx-20];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=func7[idx-25];
        }
        return;
    }

};

class I:public instructions{
    public:
    bitset<5> rd;
    bitset<3> func3;
    bitset<5> rs1;
    bitset<12> imm;
    
    void fetchInstructionDetails(string line,int i){
        while(line[i]!='x'){
            i++;
        }
        i++;
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        int registerValueInDecimal=stoi(registerValue);
        rd=registerValueInDecimal;
        while(line[i]!='x'){
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        registerValueInDecimal=stoi(registerValue);
        rs1=registerValueInDecimal;
        while(!isdigit(line[i])){
            i++;
        }
        string immValue="";
        while(line[i]!=' ' and line[i]!=','){
            immValue+=line[i];
            i++;
        }
        int immValueInDecimal=stoi(immValue);
        imm=immValueInDecimal;
        return;
    }

    void convertTo32bit(){
        int idx=0;
        for(;idx<7;idx++){
            instructionIn32bit[idx]=opcode[idx];
        }
        for(;idx<12;idx++){
            instructionIn32bit[idx]=rd[idx-7];
        }
        for(;idx<15;idx++){
            instructionIn32bit[idx]=func3[idx-12];
        }
        for(;idx<20;idx++){
            instructionIn32bit[idx]=rs1[idx-15];
        }
        for(;idx<32;idx++){
            instructionIn32bit[idx]=imm[idx-20];
        }
        return;
    }

};

class add:public R{
    public:
    add(){
        this->opcode=51;
        this->func3=0;
        this->func7=0;
    }
};

class And:public R{    // since and is a keyword so Add name is given
    public:
    And(){
        this->opcode=51;
        this->func3=7;
        this->func7=0;
    }
};

class andi: public I{
    public:
    andi(){
        opcode=19;
        func3=7;
    }
};

class makeMC{
    public:

    instructions *in=new instructions();

    string checkInstruction(string line){
        int i=0;
        string instruct="";
        while(line[i]!=' '){
            instruct+=line[i];
            i++;
        }
        if(instruct=="add"){
            add *addInstruction = new add();
            addInstruction->fetchInstructionDetails(line,i);
            addInstruction->convertTo32bit();
            addInstruction->binToHex();
            return addInstruction->instructionInHex ;
        }
        else if(instruct=="andi"){
            andi *andiInstruction = new andi();
            andiInstruction->fetchInstructionDetails(line,i);
            andiInstruction->convertTo32bit();
            andiInstruction->binToHex();
            return andiInstruction->instructionInHex;
        }
        if(instruct=="and"){
            And *andInstruction = new And();
            andInstruction->fetchInstructionDetails(line,i);
            andInstruction->convertTo32bit();
            andInstruction->binToHex();
            return andInstruction->instructionInHex ;
        }
        else return "error";
    }

    void read(){
        ifstream fin;
        ofstream fout;
        string line;
        fin.open("input.asm");
        fout.open("output.mc");
        while(getline(fin,line)){
            fout<<checkInstruction(line)<<endl;
        }
        fin.close();
        fout.close();
    }
};

int main(){
    makeMC *ne=new makeMC();
    ne->read();
    return 0;
}
