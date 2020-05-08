#include "c2.h"
#include<stdio.h>
#include<error.h>
#include<string.h>
#include<stdlib.h>
dicnode dictionary[4096]; //Arra of structures
unsigned char bitCount=0; //Used to count how many bits written
unsigned char bitBuffer=0; ////Used to write a byte to file
int index1=0; //will point to current empty index in dictionary
int lzw(char * ipfile, char * opfile){
	FILE * fin;
	FILE * fout;
	int code;                           //code for current string 
   	int c,i,dicidx;                          
	if ((fin = fopen(ipfile, "rb")) == NULL) {
        	perror("Failed to open input file");
        	return -1;
    	}
    	if ((fout = fopen(opfile, "wb")) == NULL) {
        	perror("Failed to open output file");
        	return -1;
    	}
	while(index1<256){//initialising first 256 ascii values
		dictionary[index1].codeword=index1;
		index1++;
	}
	for(i=256;i<4096;i++){//initialising rest of dictionary
		dictionary[i].codeword=-1;
	}
	code=fgetc(fin);
	while ((c = fgetc(fin)) != EOF){
		dicidx=checkdic(code,c);
		if(dicidx < 4096 && dictionary[dicidx].codeword!=-1){
			code=dictionary[dicidx].codeword; //code + c in dictionary
		}
		else{
			putcode(code,fout);
			if(index1<4096){
				dictionary[dicidx].codeword=dicidx;
				dictionary[dicidx].prefixcode=code;
				dictionary[dicidx].suffixchar=c;
				index1++;
			}
			code=c;
		}
	}
	putcode(code,fout);
	fclose(fin);
	fclose(fout);
	return 0;
}			
int checkdic(unsigned int prefixCode, unsigned char c){ //searches if prefixcode + char c are in dictionary)
	int i,idx=-1;
	for(i=0;i<index1;i++){
		if((dictionary[i].prefixcode==prefixCode)&&(dictionary[i].suffixchar==c)){
			idx=dictionary[i].codeword;
			return idx;;
		}
	}
	/*if(idx==-1){//Couldnt find code so return an empty dictionary index
		idx=index1;
	}*/
	return index1;
}
void putcode(int code, FILE * fout){ //Writes the code to file(12 bit code)
    	unsigned char byte;
    	byte = code & 0xFF; //Writes least Significant byte
    	putachar(byte, fout);
    	byte = (code >> 4) & (255 << 4); //Writes remaining bits (itll be of the form first 4 bits are the remaining bits and last 4 are 0s)
    	putbits(fout, &byte, 4);
}
int putachar(int c, FILE * fp){
    	unsigned char tmp;
    	if (bitCount == 0){//Just put byte from file
        	return fputc(c, fp);
    	}
    	tmp = ((unsigned char)c) >> (bitCount); //Takes only that many bits that can be fit insde buffer of 8 bits
    	tmp = tmp | ((bitBuffer) << (8 - bitCount));//Appends those bits to bitbuffer
    	if (fputc(tmp, fp) != EOF){
        	bitBuffer = c;//Put the code in buffer. Count will help us determine how may bits still left to be copied into file
    	}
    	else{
        	return EOF;
    	}
    	return tmp;
}
int putbits(FILE * fp, unsigned char *bits, unsigned int count){
    	unsigned char tmp;
    	int remaining, returnValue;
    	if (bits == NULL){
        	return(EOF);
    	}
    	remaining = count;
    	tmp = *bits; //Write bits 
    	while (remaining > 0){
        	returnValue = putbit((tmp & 0x80), fp);// Gives MSB
        	if (returnValue == EOF){
            		return EOF;
        	}
        	tmp <<= 1; //Left shift will occur 4 times to get 1st 4 bits in buffer(Shifts MSB)
        	remaining--;
    	}
    	return count;
}
int putbit(int c, FILE * fp){
    	int returnValue = c;
    	bitCount++;
    	bitBuffer <<= 1;
    	if (c != 0){
        	bitBuffer |= 1;
    	}
    	if (bitCount == 8){//Write bit buffer
        	if (fputc(bitBuffer, fp) == EOF){
            		returnValue = EOF;
        	}
        	bitCount = 0;
        	bitBuffer = 0;
    	}
    	return returnValue;
}

