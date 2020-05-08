#include<stdio.h>
typedef struct dicnode1{
	int codeword;////used further in dictionary when you start adding newer strings to  dictionary. Will point to index of prefixcode
	unsigned char suffixchar;//Will add prefixstring
    	unsigned int prefixcode;// The appended char to string
}dicnode1;

int delzw(char * ipfile, char * opfile);//Main calls this along with file names. 
int getcode(FILE *fin);//Gets 12 bits from file
unsigned char decode(unsigned int code, FILE *fout);//decode function

//Functions for reading 12 bits from file at a time

int getbit(FILE * fp);
int getbits(FILE * fp, unsigned char *bits, unsigned int count); 
int getachar(FILE * fp); 


