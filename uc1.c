#include "uc1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
unsigned int originalsize1 = 0; //used in header stored in the beginning of compressed file. Gives number of characters in file.
int usedchar1 = 0; //Gives the number of characters that are present in sentence
node *nodes1 = NULL; //Will store the characters used and their frequency in sorted order
unsigned char buffer1[1]; //Stores byte to read code
int bitsinbuffer1 = 0; //Will check how many bits read
int currentbit1 = 0; //Current bit read
int eof_input = 0; //Once end of file is encountered becomes 1

int dehuffman(char* ipfile, char *opfile){ 
	FILE *fin, *fout;
	if ((fin = fopen(ipfile, "rb")) == NULL) {
	        perror("Failed to open input file");
	        return -1;
    	}
    	if ((fout = fopen(opfile, "wb")) == NULL) {
        	perror("Failed to open output file");
        	fclose(fin);
        	return -1;
    	}
	if (readheader(fin) == 0) {
	        buildhuffmancomptree1(fin,fout);
    	}
	//fputc(10,fout); //The gedit newline issue
    	fclose(fin);
    	fclose(fout);
   	free(nodes1);
	return 0;
}
int readheader(FILE *f){
     	int i, j, byte = 0, size;
     	unsigned char buff[4];
     	fread(&buff, 1, sizeof(int), f);
     	byte = 0;
     	originalsize1 = buff[byte++];
     	while (byte < sizeof(int))
         	originalsize1 = (originalsize1 << 8) | buff[byte++];
     	fread(&buff, 1, sizeof(int), f);
     	byte=0;
     	usedchar1 = buff[byte++];
     	while(byte < sizeof(int))
		usedchar1 = (usedchar1 << 8) | buff[byte++];
	nodes1 = (node *)malloc(usedchar1 *(sizeof(node)));
	if(nodes1 == NULL)
		return -1;
     	size = usedchar1 * (1 + sizeof(int));
     	unsigned int weight;
     	char *buffer2 = (char *)malloc(size*(sizeof(char)));
     	if (buffer2 == NULL)
         	return -1;
     	fread(buffer2, 1, size, f);
     	byte = 0;
     	for (i = 0; i < usedchar1; ++i) {
         	nodes1[i].index = (buffer2[byte++]);
        	j = 0;
        	weight = (unsigned char) buffer2[byte++];
        	while (++j < sizeof(int)){
        	     	weight = (weight << 8) | (unsigned char) buffer2[byte++];
        	}
         	nodes1[i].weight = weight;
     	}
     	free(buffer2);
     	return 0;
}
void buildhuffmancomptree1(FILE *fin, FILE * fout){
	int i;
	char * arr=(char *)malloc(usedchar1 * (sizeof(char)));
	int * freq=(int *)malloc(usedchar1 * (sizeof(int)));
	for(i=0;i<usedchar1;i++){
		arr[i]=nodes1[i].index;
		freq[i]=nodes1[i].weight;
	}
	int size = usedchar1;  
	treenode* root = buildHuffmanTree(arr, freq, size);
	decodebit(fin , fout, root);
}
void decodebit(FILE *fin, FILE *fout,treenode* root){
	int i = 0, bit;
	struct treenode* temp;
	temp = root;
	bit = readbit(fin);
    	while (1){
		if(bit == 0 && (temp->left!=NULL)){
			temp=temp->left;
			bit = readbit(fin);
			continue;
		}
		else if(bit == 1 && (temp->right!=NULL)){
                	temp=temp->right;
			bit = readbit(fin);
			continue;
		}
		else if(bit==-1||i==originalsize1){
			break;
		}
		else if(temp->right==NULL && temp->left==NULL){
            		char c = temp->data;
            		fputc(c,fout);
	    		i++;
	    		temp=root;
			continue;
		}
        }
}
int readbit(FILE *f){
	if (currentbit1 == bitsinbuffer1){
        	if (eof_input)
            		return -1;
        	else {
            		fread(buffer1, 1, 1, f);
                	if (feof(f))
                    		eof_input = 1;
            		bitsinbuffer1 = 8;
            		currentbit1 = 0;
        	}
    	}
    	if (bitsinbuffer1 == 0)
        	return -1;
    	int bit=((buffer1[0] & (1 << (7 - currentbit1)))!=0); 
    	currentbit1++;
    	return bit;
}
treenode* newNode1(char data, unsigned freq){ 
	treenode* temp = (treenode*)malloc(sizeof(treenode)); 
	temp->left = temp->right = NULL; 
	temp->data = data; 
	temp->freq = freq;
	return temp; 
} 
tree* createtree1(unsigned capacity){ 
	tree* hufftree = (tree*)malloc(sizeof(tree)); 
	hufftree->size = 0; 
	hufftree->array = (treenode**)malloc(capacity * sizeof(treenode*)); 
	return hufftree; 
} 
void minHeapify1(tree* hufftree, int idx){ 
	int smallest = idx; 
	int left = 2 * idx + 1; 
	int right = 2 * idx + 2; 
	if (left < hufftree->size && hufftree->array[left]-> freq < hufftree->array[smallest]->freq) 
		smallest = left; 
	if (right < hufftree->size && hufftree->array[right]->freq < hufftree->array[smallest]->freq) 
		smallest = right; 
	if (smallest != idx){ 
		treenode* temp=hufftree->array[smallest];
		hufftree->array[smallest]=hufftree->array[idx];
		hufftree->array[idx]=temp; 	
		minHeapify1(hufftree, smallest); 
	} 
} 
treenode* extractMin1(tree* hufftree) { 
	treenode* temp = hufftree->array[0]; 
	hufftree->array[0] = hufftree->array[hufftree->size - 1]; 
	--hufftree->size; 
	minHeapify(hufftree, 0); 
	return temp; 
} 
void inserttree1(tree* hufftree,treenode* hufftreenode){ 
	hufftree->size++; 
	int i = hufftree->size - 1; 
	while (i && hufftreenode->freq < hufftree->array[(i - 1) / 2]->freq) { 
		hufftree->array[i] = hufftree->array[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 
	hufftree->array[i] = hufftreenode; 
} 
tree* createAndBuildtree1(char data[], int freq[], int size){ 
	tree* hufftree = createtree1(size); 
	for (int i = 0; i < size; ++i){ 
		hufftree->array[i] = newNode(data[i], freq[i]); 
	}
	hufftree->size = size; 
	return hufftree; 
} 
treenode* buildHuffmanTree1(char data[], int freq[], int size) { 
	treenode *left, *right, *top; 
	tree* hufftree = createAndBuildtree1(data, freq, size); 
	while (!(hufftree->size==1)) { 
		left = extractMin(hufftree); 
		right = extractMin(hufftree); 
		// '$' is a special value for internal nodes, not used 
		top = newNode('$', left->freq + right->freq); 
		top->left = left; 
		top->right = right; 
		inserttree1(hufftree, top); 
	}  
	return extractMin1(hufftree); 
} 
				
					

