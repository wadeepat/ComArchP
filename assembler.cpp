#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <fstream>
#include <iomanip>

using namespace std;

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
////////////////////////////// Funtion for each instcution type ///////////////////////////
void rtype(char *, char *, char *, char *, char *);
void itype(char *, char *, char *, char *, char *);
void jtype(char *, char *, char *, char *, char *);
void otype(char *, char *, char *, char *, char *);
///////////////////////////// Additional functions //////////////////////////////////////
void tranbin(char *, char *);
void twoCom(char *, char *);
int binTowcomdec();

int addr; string bin; string lb[1000][6]; 
int loop;

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    
    FILE *inFilePtr, *outFilePtr;
    char    label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) { // input argument must has 1.main file, 2.inputting assembly-code file, 3.outputting the machine-code file
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1]; outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }  

    
    //get all label and its address
    string text;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        text = label;
        lb[addr][0] = text;
        addr ++;
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    
    /* after doing a readAndParse, you may want to do the following to test the
        opcode */

    //check instruction and type
    int dec;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        bin.empty();
        dec = 0;
        if(!strcmp(opcode, "add")||!strcmp(opcode, "nand")){
            rtype(label, opcode, arg0, arg1, arg2); //add and nand
        }else if(!strcmp(opcode, "lw")||!strcmp(opcode, "sw")||!strcmp(opcode, "beq")){
             itype(label, opcode, arg0, arg1, arg2); //lw, sw and beq
        }else if(!strcmp(opcode, "jalr")) {
            jtype(label, opcode, arg0, arg1, arg2);//jarl
        }else if(!strcmp(opcode, "halt")||!strcmp(opcode, "noop")){
            otype(label, opcode, arg0, arg1, arg2);//halt and noop
        }else if(!strcmp(opcode, ".fill")){// if arg0 is number -> make 2's complement ,if Not send its address instead arg0
            if(isNumber(arg0)){
                twoCom(opcode,arg0);
            }else{
                for(int i=0; i<addr; i++){ 
                    if(lb[i][0] == arg0){
                        sprintf(arg0, "%d", i);
                        twoCom(opcode,arg0);
                    }
                }
            }
        }

        // make  decimal of instructions
        string bi = bin;
        int size  = bi.size();
        if(bi.front() == '1'){
            int plus=1;
            for(int j=0; j < size; j++){
                if(bin.back() == '1'){ 
                    if(plus==1){
                        dec += pow(2,j);
                        plus=0;
                    }
                }else{ 
                    if(plus==1){
                        plus=1;
                    }else {
                        dec += pow(2,j);
                    }
                }                
                bin.pop_back();
            }
            bin = '\0';
            dec = -dec;
        }else{
            for(int j=0; j < size; j++){
                if(bin.back() == '1') dec += pow(2,j);
                bin.pop_back();
            }
        }
        
        cout << "(address "<< loop << "): " << setw(15) << dec << "\t\t(binary: " << bi << ")\n";//print address and binary of instruction
        fprintf(outFilePtr, "%d", dec);
        fputs ("\n",outFilePtr);// write decimal machine code to outputting file
        loop++;
    }
    fclose (outFilePtr);
    return(0);
}
/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
    
    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)return(0);
    
    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    char checkLabel[1];
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        //check label don't start with number
        checkLabel[0]=label[0];
        if(isNumber(checkLabel)){
            printf("error: label can't start with number!! \n");
            printf("error At %s\n",label);
	        exit(1);
        }
        //check label max size is 6 char
        if((strlen(label)>6)){
            printf("error: label max size is 6 char!! \n");
            printf("error At %s there are %d char\n",label,strlen(label));
	        exit(1);
        }
        //label specific case
        if(!strcmp(label, "add")||!strcmp(label, "nand")); 
        else if(!strcmp(label, "lw")||!strcmp(label, "sw")||!strcmp(label, "beq"))label[0] = '\0';
        else if(!strcmp(label, "jalr")); 
        else if(!strcmp(label, "halt")||!strcmp(label, "noop"))label[0] = '\0';
        else if(!strcmp(label, ".fill")){
            printf("error: can't use .fill in label\n");
	        exit(1);
        }else {ptr += strlen(label);


        }
    }
    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
    opcode, arg0, arg1, arg2);
    return(1);
    
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}
//////////////////////////////////////// R,I,J,O TYPE Function ///////////////////////////////////////
void rtype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    bin += "0000000";
    if(!strcmp(opcode, "add")) bin += "000";
    else bin += "001";
    tranbin(opcode,arg0);
    tranbin(opcode,arg1);
    bin += "0000000000000";
    tranbin(opcode,arg2);

}

void itype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    bin += "0000000";
    if(!strcmp(opcode, "lw")) bin += "010";
    else if(!strcmp(opcode, "sw"))bin += "011";
    else bin += "100";
    tranbin(opcode,arg0);
    tranbin(opcode,arg1);
    if(isNumber(arg2)) twoCom(opcode,arg2);
    else{
        for(int i=0; i<addr; i++){
            if(lb[i][0] == arg2){
                int n = i;
                if(loop > n) n = n-loop-1;   
                sprintf(arg2, "%d", n);
                twoCom(opcode,arg2);
            }
        }if(!isNumber(arg2)){
            cout << "error: can't find label name "<< arg2 <<"\n";
	        exit(1);
        }
        
    }
}

void jtype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    bin += "0000000";
    bin += "101";
    tranbin(opcode,arg0);
    tranbin(opcode,arg1);
    bin += "0000000000000000";

}

void otype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    bin += "0000000";
    if(!strcmp(opcode, "halt")) bin += "110";
    else if(!strcmp(opcode, "noop"))bin += "111";
    bin += "0000000000000000000000";
}


//////////////////////////////////// Additional Functions ///////////////////////////////////////////////
void tranbin(char *opcode,char *num){//convert to binary
    int n;
    sscanf(num, "%d", &n);
    string bit;
    while( n > 0 ) {
        if( n % 2 == 0 ) bit.insert( bit.begin( ), '0' );
        else bit.insert( bit.begin( ), '1' );
        n >>= 1;
    }
    while(bit.size() < 3){
        bit.insert( bit.begin( ), '0' );
    }
    bin += bit;
}

void twoCom(char *opcode,char *num){//making 2's complement
    int n;
    
    sscanf(num, "%d", &n);
    
    string bit;
    if(n > 32767 || n < -32768){
        printf("error: offset field incorrect\n");
	    exit(1);
    }else{
        if(n >= 0){
            while( n > 0 ) {
                if( n % 2 == 0 ) bit.insert( bit.begin( ), '0' );
                else bit.insert( bit.begin( ), '1' );    
                n >>= 1;
            }
            if(!strcmp(opcode, ".fill"))while(bit.size() < 32) bit.insert( bit.begin( ), '0' );
            else while(bit.size() < 16) bit.insert( bit.begin( ), '0' );
        }else{
            int plus1;// do + 1 in 2's complement
            plus1=1;
            n = -n;
            while( n > 0 ) {
                if( n % 2 == 0 ) {
                    if(plus1 == 1) bit.insert( bit.begin( ), '0' );
                    else bit.insert( bit.begin( ), '1' );
                }
                else {
                    if(plus1 == 1){
                        bit.insert( bit.begin( ), '1' );
                        plus1 = 0;
                    } 
                    else bit.insert( bit.begin( ), '0' );
                }
                n >>= 1;
            }
            if(!strcmp(opcode, ".fill"))while(bit.size() < 32) bit.insert( bit.begin( ), '1' );
            else while(bit.size() < 16) bit.insert( bit.begin( ), '1' );
        }
        bin += bit;
    }
}
