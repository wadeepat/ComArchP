#include <stdio.h>
#include <string>
#include <math.h>
#include <iostream>
using namespace std;

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int);
char* dectoBi(int,char *);
int bitoDec16bit(char*);
void iType(stateType *,char*);
void rType(stateType *,char*);


int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
     //argv[1] = "text.txt";
    //sprintf("argc = %d\n",argc);
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        //exit(1);
    }
   
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        //exit(1);
    }

    //set default reg
    for(int i=0;i<NUMREGS;i++) state.reg[i] = 0;
    //for(int i=0;i<NUMREGS;i++) printf("reg[%d]= %d",i,state.reg[i]);


    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr)!= NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            //exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    bool running= true;

   

     char bi[32]="1000000000000000";
     
    // for(int i=31;i>=0;i--){
    //     cout<<bi[i];
    // }
    //cout<<bi[31]<<endl;
    
    int count = 0; char* temp;
    while(running){
        count++;
        printState(&state);
        //cout<<state.pc<<endl;
        //cout<<"start state = " << <<endl;
        char *instruc = dectoBi(state.mem[state.pc],temp);
        //cout<<instruc<<endl;
        //cout<<"test";
        //printf("instruc= %c",instruc);
        //printf("test %c %c \n",instruc[24],instruc[23]);
        if(instruc[24]=='0' && instruc[23]=='0'){
            //r
            rType(&state,instruc);
            //cout<<"in r"<<endl;
        }else if(instruc[24]=='1'&& instruc[23]=='0' && instruc[22]=='1'){
            //j
        }else if(instruc[24]=='1'&& instruc[23]=='1'){
            //o
            if(instruc[22]=='0'){
                //halt
                state.pc++;
                cout<<"machine halted\ntotal of "<<count<<" instructions executed\nfinal state of machine:\n";
                running=false;
            }
        }else{
            //i
            //cout<<"in i"<<endl;
            iType(&state,instruc);
        }
        //printState(&state);
        
        //cout<<"count= "<<count<<endl;
        //if(count==6)running = false;
    }
    
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");

	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}

    printf("\tregisters:\n");

	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    
    printf("end state\n");
}

int convertNum(int num){
    /* convert a 16-bit number into a 32-bit integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

char* dectoBi(int dec,char *temp){
    //cout<<dec<<endl; 
    int i=0;
    
    while(i!=32){
        //cout<<temp[i]<<endl;
        if(dec!=0) {
            if(dec%2==0)temp[i]='0';
            else temp[i]='1'; 
            dec= dec/2;   
        }else temp[i]='0';
        //cout<<temp[i]<<endl;
        i++;  
    }  
    //cout<<"finished"<<endl; //for what
    return temp;
}

int bitoDec3bit(char a,char b,char c){
    int ans = 0;
    if(a == '1')ans+=4;
    if(b == '1')ans+=2;
    if(c == '1')ans+=1;
    return ans;
}

int bitoDec16bit(char* bi){
    int ans =0;
    for(int i=0;i<16;i++){
        if(bi[i]=='1')ans+= pow(2,i);
    }
    //cout<<"ok";
    return ans;
}

void rType(stateType *statePtr,char* instruc){
    
    int rd = bitoDec3bit(instruc[2],instruc[1],instruc[0]);

    if(instruc[22]=='0'){//add
        int regA = bitoDec3bit(instruc[21],instruc[20],instruc[19]);
        int regB = bitoDec3bit(instruc[18],instruc[17],instruc[16]);
        statePtr->reg[rd] = statePtr->reg[regA]+statePtr->reg[regB];
        statePtr->pc++;
    }else{//nand
        char a,b,c;
        if(instruc[21]&&instruc[18])a='0';
        else a='1';
        if(instruc[20]&&instruc[17])a='0';
        else a='1';
        if(instruc[19]&&instruc[16])a='0';
        else a='1';
    }
}

void iType(stateType *statePtr,char* instruc){
    int ra = bitoDec3bit(instruc[21],instruc[20],instruc[19]);
    int rb = bitoDec3bit(instruc[18],instruc[17],instruc[16]);
    int imm = convertNum(bitoDec16bit(instruc)); 
    //cout<<imm<<endl;
    //cout<<statePtr->mem[7]<<endl;
    if(instruc[24]== '0'){
        if(instruc[22]=='0'){
            //lw
            statePtr->reg[rb] = statePtr->mem[statePtr->reg[ra] + imm]; 
            //cout<<ra<<" "<<rb<<" "<<statePtr->reg[ra+rb]<<endl;
            statePtr->pc++;
        }else{
            //sw

        }
    }else{
        //beq
        //cout<<"beq "<<ra<<" "<<rb<<endl;
        if(statePtr->reg[ra]==statePtr->reg[rb])statePtr->pc += 1+imm;
        else statePtr->pc++;
    }
    
}
void jType(stateType *statePtr,char* instruc){

  
}
void oType(stateType *statePtr,char* instruc){

   
}









