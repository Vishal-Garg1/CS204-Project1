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
    
    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,rs1,imm
    void fetchInstructionDetailsType1(string line,int i){

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

    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,imm(rs1)
    void fetchInstructionDetailsType2(string line,int i){

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

        // fetching imm
        while(!isdigit(line[i])){
            i++;
        }
        string immValue="";
        while(line[i]!=' ' and line[i]!='('){
            immValue+=line[i];
            i++;
        }
        int immValueInDecimal=stoi(immValue);
        imm=immValueInDecimal;

        // fetching rs1
        while(line[i]!='x'){
            i++;
        }
        i++;
        registerValue="";
        while(line[i]!=' ' and line[i]!=')'){
            registerValue+=line[i];
            i++;
        }
        registerValueInDecimal=stoi(registerValue);
        rs1=registerValueInDecimal;

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

// class for or instructions inheriting R publicly
class Or:public R{    // since or is a keyword so Or name is given
    public:
    Or(){
        this->opcode=51;
        this->func3=6;
        this->func7=0;
    }
};

// class for sll instructions inheriting R publicly
class sll:public R{
    public:
    sll(){
        this->opcode=51;
        this->func3=1;
        this->func7=0;
    }
};

// class for slt instructions inheriting R publicly
class slt:public R{
    public:
    slt(){
        this->opcode=51;
        this->func3=2;
        this->func7=0;
    }
};

// class for sra instructions inheriting R publicly
class sra:public R{
    public:
    sra(){
        this->opcode=51;
        this->func3=5;
        this->func7=32;
    }
};

// class for srl instructions inheriting R publicly
class srl:public R{
    public:
    srl(){
        this->opcode=51;
        this->func3=5;
        this->func7=0;
    }
};

// class for sub instructions inheriting R publicly
class sub:public R{
    public:
    sub(){
        this->opcode=51;
        this->func3=0;
        this->func7=32;
    }
};

// class for xor instructions inheriting R publicly
class Xor:public R{     // since xor is a keyword so Xor name is given
    public:
    Xor(){
        this->opcode=51;
        this->func3=4;
        this->func7=0;
    }
};

// class for mul instructions inheriting R publicly
class mul:public R{
    public:
    mul(){
        this->opcode=51;
        this->func3=0;
        this->func7=1;
    }
};

// class for div instructions inheriting R publicly
class Div:public R{     // since div() is a predefined function so Div name is given
    public:
    Div(){
        this->opcode=51;
        this->func3=4;
        this->func7=1;
    }
};

// class for rem instructions inheriting R publicly
class rem:public R{
    public:
    rem(){
        this->opcode=51;
        this->func3=6;
        this->func7=1;
    }
};

// class for addi instructions inheriting I publicly
class addi: public I{
    public:
    addi(){
        opcode=19;
        func3=0;
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

// class for ori instructions inheriting I publicly
class ori: public I{
    public:
    ori(){
        opcode=19;
        func3=6;
    }
};

// class for lb instructions inheriting I publicly
class lb: public I{
    public:
    lb(){
        opcode=3;
        func3=0;
    }
};

// class for lh instructions inheriting I publicly
class lh: public I{
    public:
    lh(){
        opcode=3;
        func3=1;
    }
};

// class for lw instructions inheriting I publicly
class lw: public I{
    public:
    lw(){
        opcode=3;
        func3=2;
    }
};

// class for ld instructions inheriting I publicly
class ld: public I{
    public:
    ld(){
        opcode=3;
        func3=3;
    }
};

// class for jalr instructions inheriting I publicly
class jalr: public I{
    public:
    jalr(){
        opcode=103;
        func3=0;
    }
};

//class to make machine code for whole program
class makeMC{
    public:

    bitset<32> pc=0;
    string pcInHex;

    void incrementPC(){
        for(int i=2;i<32;i++){
            if(pc[i]==0){
                pc[i]=1;
                break;
            }
            else{
                pc[i]=0;
            }
        }
        return;
    }

    // function to convert 32 bit binary to hexadecimal
    void binToHexPC(){
        if(pc==0){
            pcInHex="0x0";
            return;
        }
        pcInHex="0x";   // initialising with 0x
        bool flag=0;
        // converting 4 bits at a time to hex
        for(int i=31;i>=0;){
            int num=0;
            for(int j=8;j>=1;j/=2){
                num+=pc[i]*j;
                i--;
            }
            if(num!=0)   flag=1;
            if(flag){
                if(num<10){
                    pcInHex+=num+'0';
                }else{
                    pcInHex+=num-10+'A';
                }
            }
        }
        return;
    }

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

        else if(instruct=="and"){
            And *andInstruction = new And();    // making an object of class And
            andInstruction->fetchInstructionDetails(line,i);    // fetching fields
            andInstruction->convertTo32bit();   // converting to 32 bit code
            andInstruction->binToHex();     // converting to hex
            return andInstruction->instructionInHex;   // returning mc in hex
        }

        else if(instruct=="or"){
            Or *newInstruction = new Or();    // making an object of class Or
            newInstruction->fetchInstructionDetails(line,i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if(instruct=="or"){
            Or *newInstruction = new Or();    // making an object of class And
            newInstruction->fetchInstructionDetails(line,i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "sll") {
            sll *newInstruction = new sll();    // making an object of class sll
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "slt") {
            slt *newInstruction = new slt();    // making an object of class slt
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "sra") {
            sra *newInstruction = new sra();    // making an object of class sra
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "srl") {
            srl *newInstruction = new srl();    // making an object of class srl
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "sub") {
            sub *newInstruction = new sub();    // making an object of class sub
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "xor") {
            Xor *newInstruction = new Xor();    // making an object of class Xor
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "mul") {
            mul *newInstruction = new mul();    // making an object of class mul
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "div") {
            Div *newInstruction = new Div();    // making an object of class Div
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "rem") {
            rem *newInstruction = new rem();    // making an object of class rem
            newInstruction->fetchInstructionDetails(line, i);    // fetching fields
            newInstruction->convertTo32bit();   // converting to 32 bit code
            newInstruction->binToHex();     // converting to hex
            return newInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "addi") {
            addi *addiInstruction = new addi();    // making an object of class addi
            addiInstruction->fetchInstructionDetailsType1(line, i);   // fetching fields
            addiInstruction->convertTo32bit();  // converting to 32-bit code
            addiInstruction->binToHex();    // converting to hex
            return addiInstruction->instructionInHex;   // returning mc in hex
        }

        else if(instruct=="andi"){
            andi *andiInstruction = new andi();    // making an object of class andi
            andiInstruction->fetchInstructionDetailsType1(line,i);   // fetching fields
            andiInstruction->convertTo32bit();  // converting to 32 bit code
            andiInstruction->binToHex();    // converting to hex
            return andiInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "ori") {
            ori *oriInstruction = new ori();    // making an object of class ori
            oriInstruction->fetchInstructionDetailsType1(line, i);   // fetching fields
            oriInstruction->convertTo32bit();  // converting to 32-bit code
            oriInstruction->binToHex();    // converting to hex
            return oriInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "lb") {
            lb *lbInstruction = new lb();    // making an object of class lb
            lbInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lbInstruction->convertTo32bit();  // converting to 32-bit code
            lbInstruction->binToHex();    // converting to hex
            return lbInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "ld") {
            ld *ldInstruction = new ld();    // making an object of class ld
            ldInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            ldInstruction->convertTo32bit();  // converting to 32-bit code
            ldInstruction->binToHex();    // converting to hex
            return ldInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "lh") {
            lh *lhInstruction = new lh();    // making an object of class lh
            lhInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lhInstruction->convertTo32bit();  // converting to 32-bit code
            lhInstruction->binToHex();    // converting to hex
            return lhInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "lw") {
            lw *lwInstruction = new lw();    // making an object of class lw
            lwInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            lwInstruction->convertTo32bit();  // converting to 32-bit code
            lwInstruction->binToHex();    // converting to hex
            return lwInstruction->instructionInHex;   // returning mc in hex
        }

        else if (instruct == "jalr") {
            jalr *jalrInstruction = new jalr();    // making an object of class jalr
            jalrInstruction->fetchInstructionDetailsType2(line, i);   // fetching fields
            jalrInstruction->convertTo32bit();  // converting to 32-bit code
            jalrInstruction->binToHex();    // converting to hex
            return jalrInstruction->instructionInHex;   // returning mc in hex
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
            binToHexPC();
            fout<<pcInHex<<" "<<checkInstruction(line)<<endl;      // writing to output.mc
            incrementPC();
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
