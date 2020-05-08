#include<stdio.h>
typedef struct dicnode{
	int codeword; //used further in dictionary when you start adding newer strings to  dictionary. Will point to index of prefixcode
	unsigned int prefixcode; //Will add prefixstring
	unsigned char suffixchar;// The appended char to string
}dicnode;

int lzw(char * ipfile, char * opfile); //Main calls this along with file names.
int checkdic(unsigned int prefixCode, unsigned char c); //Checks existing dictionary for string + c. If not found returns empty index of dictionary
void putcode(int code, FILE * f); //Encodes current code into file

//Code for writing 12 bits to file. Reffered to standard bit manipulation file library.

int putbit(int c, FILE * fp); // Puts a single bit
int putbits(FILE * stream, unsigned char * bits, unsigned int count); //Puts a couple of bits
int putachar(int c, FILE * fp); //Puts a whole character(ie 1 byte);






