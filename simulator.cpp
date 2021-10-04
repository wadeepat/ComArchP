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
void jType(stateType *,char*);


int main(int argc, char *argv[]){
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
     
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
   
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    //set default reg
    for(int i=0;i<NUMREGS;i++) state.reg[i] = 0;

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr)!= NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    state.numMemory+=5;
    bool running= true;
    
    int count = 0;//count how many instruction we have been run 
    char* temp;
    while(running){
        count++;
        printState(&state);
        char *instruc = dectoBi(state.mem[state.pc],temp);
        if(instruc[24]=='0' && instruc[23]=='0'){//r
            rType(&state,instruc);
        }else if(instruc[24]=='1'&& instruc[23]=='0' && instruc[22]=='1'){//j
            jType(&state,instruc);
        }else if(instruc[24]=='1'&& instruc[23]=='1'){//o
            if(instruc[22]=='0'){//halt
                state.pc++;
                cout<<"machine halted\ntotal of "<<count<<" instructions executed\nfinal state of machine:\n";
                running=false;
            }
        }else{//i
            iType(&state,instruc);
        }
    }
    
    printState(&state);
    
    return(0);
}

void printState(stateType *statePtr){
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
    int i = 0;
    
    while(i!=32){
        if(dec!=0) {
            if(dec%2==0)temp[i]='0'; 
            else temp[i]='1'; 
            dec= dec/2;   
        }else temp[i]='0';
        i++;  
    }  
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
    return ans;
}

void rType(stateType *statePtr,char* instruc){
    int regA = bitoDec3bit(instruc[21],instruc[20],instruc[19]);//dec id
    int regB = bitoDec3bit(instruc[18],instruc[17],instruc[16]);//dec id
    int rd = bitoDec3bit(instruc[2],instruc[1],instruc[0]);

    if(instruc[22]=='0'){//add
        
        statePtr->reg[rd] = statePtr->reg[regA]+statePtr->reg[regB];
    }else{//nand
       
        char temp[16];
        int a = statePtr->reg[regA]; 
        int b = statePtr->reg[regB]; 
        
        for(int j=0;j<=14;j++){
            if(a%2 == 1 && b%2 == 1)temp[j]='0'; 
            else temp[j]='1';                    
            a=a/2;
            b=b/2;
        }
   
        statePtr->reg[rd] = bitoDec16bit(temp);
    }
    statePtr->pc++;
}

void iType(stateType *statePtr,char* instruc){
    int ra = bitoDec3bit(instruc[21],instruc[20],instruc[19]);
    int rb = bitoDec3bit(instruc[18],instruc[17],instruc[16]);
    int imm = convertNum(bitoDec16bit(instruc)); 
    
    if(instruc[24]== '0'){
        if(instruc[22]=='0'){//lw
            statePtr->reg[rb] = statePtr->mem[statePtr->reg[ra] + imm]; 
            statePtr->pc++;
        }else{//sw
            //mem[offsetField+reg[a]] = reg[b]
            statePtr->mem[statePtr->reg[ra]+imm] = statePtr->reg[rb]; 
            statePtr->pc++;
        }
    }else{//beq
        
        if(statePtr->reg[ra]==statePtr->reg[rb])statePtr->pc += 1+imm;//pc = pc+1+offsetField
        else statePtr->pc++;
    }

    
}

void jType(stateType *statePtr,char* instruc){
    //jalr
    int ra = bitoDec3bit(instruc[21],instruc[20],instruc[19]);
    int rb = bitoDec3bit(instruc[18],instruc[17],instruc[16]);

    statePtr->reg[rb] = statePtr->pc + 1; //reg[b] = pc+1 for return

    if(ra!=rb){
        statePtr->pc = statePtr->reg[ra];//jump to pc = reg[a]
    }else{
        statePtr->pc++;//jump to pc = pc + 1 
    }
}









