#include "uc2.h"
#include<stdio.h>
#include<error.h>
#include<string.h>
#include<stdlib.h>
dicnode1 dictionary[4096];
unsigned char bitCount1=0;
unsigned char bitBuffer1=0;
int index2=0; //will point to current empty index in dictionary
int delzw(char * ipfile, char * opfile){
	FILE * fin;
	FILE * fout;
	unsigned int code;// code for current string 
   	int c,i,dicidx;  
	unsigned int lastcode;                          
	if ((fin = fopen(ipfile, "rb")) == NULL) {
        	perror("Failed to open input file");
        	return -1;
    	}
    	if ((fout = fopen(opfile, "wb")) == NULL) {
        	perror("Failed to open output file");
        	return -1;
    	}
	while(index2<256){//initialising first 256 ascii values
		dictionary[index2].codeword=index2;
		index2++;
	}
	for(i=256;i<4096;i++){//initialising rest of dictionary
		dictionary[i].codeword=-1;
	}
	lastcode=getcode(fin); //get first character of file
	c=lastcode;
	fputc(lastcode,fout); //Output that
	while ((code = getcode(fin)) != EOF){
        	if (code < index2){
        		c = decode(code, fout); //known code(already in dictionary)
        	}
        	else{// Only for string + char + string + char exception
            	 	unsigned char tmp;
			tmp = c;
            		c = decode(lastcode, fout);
            		fputc(tmp, fout); //Output prev code
        	}
       	 	if (index2 < 4096){ //Add code + c combination to dictionary
            		dictionary[index2].prefixcode = lastcode;
            		dictionary[index2].suffixchar = c;
            		index2++;
        	}
        	lastcode = code;
    	}
	fclose(fout);
    	fclose(fin);
    	return 0;
}
unsigned char decode(unsigned int code, FILE *fout){
	unsigned char c;
	unsigned char firstchar;
    	if (code >= 256){//Code is some string(already has a code) + a character
        	c = dictionary[code].suffixchar;
        	code = dictionary[code].prefixcode;
        	firstchar = decode(code, fout);//Try to go back recursively till you get a single char and keep on outputting until the 
    	}                                      //whole string is printed in the output file.
    	else{//Reached a point where code is just a character. Or it was a single chracter in the frst place
        	c = code;
        	firstchar = code;
    	}
    	fputc(c, fout);  
    	return firstchar;
}
int getcode(FILE *fin){
	unsigned char byte;
    	int code;
    	if ((code = getachar(fin)) == EOF){ //Gets Last Significant byte
        	return EOF;
    	}
    	if (getbits(fin, &byte, 4) == EOF){//Gets remaining bits
        	return EOF;
    	}
    	code |= ((int)byte) << 4; //Converts to proper integer of 12 bits
    	return code;
}
int getachar(FILE * fp){
	int returnValue;
    	unsigned char tmp;
    	returnValue = fgetc(fp);
    	if (bitCount1 == 0){ //We can get directly send a byte from file
        	return returnValue;
    	}
    	if (returnValue != EOF){//We have some buffered bits to return. (So figure out what to return)
        	tmp = ((unsigned char)returnValue) >> (bitCount1); //Gets only the number of bits that can fit in buffer
        	tmp |= ((bitBuffer1) << (8 - (bitCount1)));//Appends to tmp
        	bitBuffer1 = returnValue;//Adds the code to buffer again as there are still some bits left to be read & count wouldnt change
        	returnValue = tmp;
    	}
    	return returnValue;
}
int getbits(FILE * fp, unsigned char *bits, unsigned int count){
	unsigned char shifts;
    	int remaining, returnValue;
    	if (bits == NULL){
        	return(EOF);
    	}
    	remaining = count;
    	while (remaining > 0){
        	returnValue = getbit(fp);
        	if (returnValue == EOF){
            		return EOF;
        	}
        	*bits <<= 1;
        	*bits |= (returnValue & 0x01);
        	remaining--;
    	}
    	*bits <<= 4;//Shift last bits into position
    	return count;
}
int getbit(FILE * fp){
	int returnValue;
    	if (bitCount1 == 0){
        	if ((returnValue = fgetc(fp)) == EOF){//Buffer is empty so read another character
            		return EOF;
        	}
        	else{
            		bitCount1 = 8;
            		bitBuffer1 = returnValue;
        	}
    	}
    	//Bit to return is MSB(in buffer)	
    	bitCount1--;
    	returnValue = (bitBuffer1) >> (bitCount1);
    	return (returnValue & 0x01); //Returns MSB 
}

