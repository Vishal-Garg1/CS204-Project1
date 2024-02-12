#include<iostream>
#include<fstream>  // for file handling
#include<bitset>  
using namespace std;

// main class for all the instructions
class instructions{
    public:
    bitset<32> instructionIn32bit;   // to store instuction code in binary
    string instructionInHex;  // to store instuction code in hexadecimal
    bitset<7> opcode;    // opcode for the instructions

    // function to convert 32 bit binary to hexadecimal
    void binToHex(){
        instructionInHex="0x";   // initialising with 0x
        // converting 4 bits at a time to hex
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

// class for R type instructions inheriting instuctions class publicly
class R:public instructions{
    public:
    bitset<5> rd;       // to store destination register value
    bitset<3> func3;    // to store func3 value
    bitset<5> rs1;      // to store source register 1 value
    bitset<5> rs2;      // to store source register 2 value
    bitset<7> func7;    // to store func7 value
    
    // function to fetch rd,rs1,rs2 fields of the R type instruction
    void fetchInstructionDetails(string line,int i){
        while(line[i]!='x'){
            i++;
        }
        i++;

        // fetching rd
        string registerValue="";
        while(line[i]!=' ' and line[i]!=','){
            registerValue+=line[i];
            i++;
        }
        int registerValueInDecimal=stoi(registerValue);  // string to int conversion
        rd=registerValueInDecimal;

        // fetching rs1
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

        // fetching rs2
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

    // function to make instruction to binary 32 bit format joining all different fields
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

// class for I type instructions inheriting instuctions class publicly
class I:public instructions{
    public:
    bitset<5> rd;         // to store destination register value
    bitset<3> func3;      // to store func3 value
    bitset<5> rs1;        // to store source register 1 value
    bitset<12> imm;       // to store immediate field value
    
    // function to fetch rd,rs1,rs2 fields of the I type instruction
    void fetchInstructionDetails(string line,int i){

        //fetching rd
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

        // fetching rs1
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

        // fetching imm
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

    // function to make instruction to binary 32 bit format joining all different fields
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

// class for add instructions inheriting R publicly
class add:public R{
    public:
    add(){
        this->opcode=51;
        this->func3=0;
        this->func7=0;
    }
};

// class for and instructions inheriting R publicly
class And:public R{    // since and is a keyword so And name is given
    public:
    And(){
        this->opcode=51;
        this->func3=7;
        this->func7=0;
    }
};

// class for andi instructions inheriting I publicly
class andi: public I{
    public:
    andi(){
        opcode=19;
        func3=7;
    }
};

//class to make machine code for whole program
class makeMC{
    public:

    // function to check if the instruction is right or not and which instruction is it
    string checkInstruction(string line){
        int i=0;
        // fetching instruction name in instruct
        string instruct="";
        while(line[i]!=' '){
            instruct+=line[i];
            i++;
        }

        if(instruct=="add"){
            add *addInstruction = new add();    // making an object of class add
            addInstruction->fetchInstructionDetails(line,i);  // fetching fields
            addInstruction->convertTo32bit();   // converting to 32 bit code
            addInstruction->binToHex();   // converting to hex
            return addInstruction->instructionInHex ;   // returning mc in hex
        }

        else if(instruct=="andi"){
            andi *andiInstruction = new andi();    // making an object of class andi
            andiInstruction->fetchInstructionDetails(line,i);   // fetching fields
            andiInstruction->convertTo32bit();  // converting to 32 bit code
            andiInstruction->binToHex();    // converting to hex
            return andiInstruction->instructionInHex;   // returning mc in hex
        }

        else if(instruct=="and"){
            And *andInstruction = new And();    // making an object of class And
            andInstruction->fetchInstructionDetails(line,i);    // fetching fields
            andInstruction->convertTo32bit();   // converting to 32 bit code
            andInstruction->binToHex();     // converting to hex
            return andInstruction->instructionInHex;   // returning mc in hex
        }

        else return "error";   // return error if instruction doesnot match to any name
    }

    // function to read from .asm file and rite to .mc file
    void readAndWrite(){
        ifstream fin;    // to read a file
        ofstream fout;   // to write to a file
        string line;
        fin.open("input.asm");       // open input.asm to read
        fout.open("output.mc");     // open output.mc to write
        while(getline(fin,line)){
            fout<<checkInstruction(line)<<endl;      // writing to output.mc
        }
        fin.close();    // close input.asm
        fout.close();   // close output.mc
    }
};

int main(){
    makeMC *newMC=new makeMC();    // making object for makeMC class
    newMC->readAndWrite();
    return 0;
}
