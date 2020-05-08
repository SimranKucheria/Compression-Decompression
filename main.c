#include<stdio.h>
#include<errno.h>
#include<string.h>
//#include "c1.h"
#include "c2.h"
#include "uc1.h"
#include "uc2.h"

int main(int argc, char * argv[]){
	int fd,i=0;
	if(argc<4){
		errno=EINVAL;
		perror("Bad Argument");
		return errno;
	}
	else{
		if((strcmp(argv[1],"c1")==0)){//Huffman Compression
			huffman(argv[2],argv[3]);
		}
		else if((strcmp(argv[1],"c2")==0)){
			lzw(argv[2],argv[3]);
		}
		else if((strcmp(argv[1],"uc1")==0)){
			dehuffman(argv[2],argv[3]);
		}
		else if((strcmp(argv[1],"uc2")==0)){
			delzw(argv[2],argv[3]);
		}
		else{
			perror("Entered a wrong argument. Run Program again with right arguments");
			return errno;
		}
	}
}

	


