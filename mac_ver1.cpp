#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

void rtype(char *, char *, char *, char *, char *);
void itype(char *, char *, char *, char *, char *);
void jtype(char *, char *, char *, char *, char *);
void otype(char *, char *, char *, char *, char *);

void tranbin(char *);
void twoCom(char *);
void binTodec(char *);

int address;
string bin{};
queue<string> symAdd;



int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

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
    
    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    int dec;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
        bin = '\0';
        dec = 0;
        if(!strcmp(opcode, "add")||!strcmp(opcode, "nand")) rtype(label, opcode, arg0, arg1, arg2); //add and nand
        else if(!strcmp(opcode, "lw")||!strcmp(opcode, "sw")||!strcmp(opcode, "beq")) itype(label, opcode, arg0, arg1, arg2); //lw, sw and beq
        else if(!strcmp(opcode, "jalr")) jtype(label, opcode, arg0, arg1, arg2);
        else if(!strcmp(opcode, "halt")||!strcmp(opcode, "noop")) otype(label, opcode, arg0, arg1, arg2);
        else;
        cout << bin << "\n";
        if(bin.size() == 26){
            int size = bin.size();
                for(int j=0; j < size; j++){
                    if(bin.back() == '1')dec += pow(2,j);
                    bin.pop_back();
                }
            // cout << bin << "\n";
            cout << dec << "\n";
        }
        
        
        address ++;

    }   
    
    // for(int i=0; i < address; i++){
    //     int dec = 0;
    //     bin = qbin.front();
    //     qbin.pop();
    //     for(int j=0; j < 25; j++){
    //         if(bin.back() == '1')dec += pow(2,j);
    //         bin.pop_back();
    //     }
    //     cout << dec << "\n";
    // }
    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    /* after doing a readAndParse, you may want to do the following to test the
//        opcode */
//    if (!strcmp(opcode, "add")) {
//        /* do whatever you need to do for opcode "add" */
//    }

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
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {// get 1 line
	/* reached end of file */
        return(0);
    }
    cout <<line<< "\n";
    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    
    ptr = line;
    
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        //check instruction and type *******ยังไม่ได้เช็คว่าเกิน6ไหม******** 
        if(!strcmp(label, "add")||!strcmp(label, "nand")); else if(!strcmp(label, "lw")||!strcmp(label, "sw")||!strcmp(label, "beq"));
        else if(!strcmp(label, "jalr")); else if(!strcmp(label, "halt")||!strcmp(label, "noop"));
        else if(!strcmp(label, ".fill"));
        else ptr += strlen(label);
    }
    

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    //sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
    //    opcode, arg0, arg1, arg2);
    sscanf(ptr, "%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",   opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

void rtype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){

    if(!strcmp(opcode, "add")) bin += "000";
    else bin += "001";
    tranbin(arg0);
    tranbin(arg1);
    bin += "0000000000000";
    tranbin(arg2);

}

void itype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    if(!strcmp(opcode, "lw")) bin += "010";
    else if(!strcmp(opcode, "sw"))bin += "011";
    else bin += "100";
    tranbin(arg0);
    tranbin(arg1);
    if(isNumber(arg2)) twoCom(arg2);
    else symAdd.push(arg2);
}

void jtype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    bin += "101";
    tranbin(arg0);
    tranbin(arg1);
    bin += "0000000000000000";

}

void otype(char *label, char *opcode, char *arg0, char *arg1, char *arg2){
    if(!strcmp(opcode, "halt")) bin += "110";
    else if(!strcmp(opcode, "noop"))bin += "111";
    bin += "0000000000000000000000";
}




void tranbin(char *num){
    int n;
    sscanf(num, "%d", &n);
    string bit{};
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

void twoCom(char *arg2){
    int n;
    sscanf(arg2, "%d", &n);
    string bit{};
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
            while(bit.size() < 16){
                bit.insert( bit.begin( ), '0' );
            }
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
            while(bit.size() < 16){
                bit.insert( bit.begin( ), '1' );
            }
        }
        bin += bit;
    }
}

