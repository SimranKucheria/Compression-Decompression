#include "c1.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
unsigned int originalsize=0; //used in header stored in the beginning of compressed file. Gives number of characters in file.
int usedchar = 0; //Gives the number of characters that are present in sentence
int *freqarr = NULL; //Will be called ahead to create an array that'll store frequency and index of character
node *nodes = NULL; //Will store the characters used and their frequency in sorted order
int numnodes=0; //will store currennt index of node
int arrofcodes[256][100]={}; //Will store the huffman codes for each of the 256 characters
void initcode(treenode* root); //Function to initialize code of array
void savecodes(treenode* root, int arr[], int top); //Function to get codes from traversing huffman tree and save it into array
int flushbuffer(FILE *f); //Function for emptying buffer out completely
unsigned char buffer[1]; //Buffer to keep bits(codes)
int bitsinbuffer = 0; //Keeps track of bits already filled
int stack[100]; //Will store the code of character to be encoded

int huffman(char * ipfile, char * opfile){
	FILE * fin;
	FILE * fout;
	if ((fin = fopen(ipfile, "rb")) == NULL){
        	perror("Failed to open input file");
        	return -1;
    	}
    	if ((fout = fopen(opfile, "wb")) == NULL) {
        	perror("Failed to open output file");
        	return -1;
    	}
	findfreq(fin);  
	nodes=(node*)malloc(256 *(sizeof(node)));
	addleaves();
	writeheader(fout);
	buildhuffmancomptree();
	fseek(fin, 0, SEEK_SET);
	int c;
   	while ((c = fgetc(fin)) != EOF){
        	encodealphabet(fout, c);
	}
	flushbuffer(fout);
	fclose(fin);
	fclose(fout);
	return 0;
}
void findfreq(FILE * f){
	freqarr=(int *)malloc(256 * (sizeof(int)));
	int ch;
	while (1) {
        	ch = fgetc(f);
        	if (ch == EOF)
            		break;
        	freqarr[ch]++;
		originalsize++;
    	}
	for(ch=0;ch<256;ch++){
     		if (freqarr[ch] > 0)
			usedchar++;
	}
}
void addleaves() {
	int i, freq;
    	for (i = 0; i < 256; ++i) {
        	freq = freqarr[i];
        	if (freq > 0)
            		addnode(i, freq);
    	}
}
void addnode(char index, int weight) {
    	int i = numnodes;
    	while (i > 0 && nodes[i-1].weight > weight) {
        	memcpy(&nodes[i], &nodes[i-1], sizeof(node));
		i--;
    	}
    	nodes[i].index = index;
    	nodes[i].weight = weight;
    	numnodes++;
}
int writeheader(FILE *f) {
     	int i, j, byte = 0,
     	size = sizeof(unsigned int) + sizeof(int) + (usedchar * (1 + sizeof(int)));
     	unsigned int weight;
     	char *buffer = (char *)malloc(size* (sizeof(char)));
     	if (buffer == NULL)
         	return -1;
     	j = sizeof(int);
	while (j--){
        	buffer[byte++] =(originalsize >> (j << 3)) & 0xff; //Will store last 8 bits of integer byte. Little Endian
	}
     	j=sizeof(int);
	while(j--){
		buffer[byte++]=(usedchar >> (j<< 3)) & 0xff;
	}
	for (i = 0; i < usedchar; ++i) {
       	 	weight = nodes[i].weight;
       	 	buffer[byte++] =nodes[i].index;
       		j = sizeof(int);
         	while (j--){
             		buffer[byte++] = (weight >> (j << 3)) & 0xff; //Will store weight also according to little endian 
		}
	}
     	fwrite(buffer, 1, size, f);
     	free(buffer);
     	return 0;
}
void buildhuffmancomptree(){
	int i;
	char * arr=(char *)malloc(usedchar * (sizeof(char)));
	int * freq=(int *)malloc(usedchar * (sizeof(int)));
	for(i=0;i<usedchar;i++){
		arr[i]=nodes[i].index;
		freq[i]=nodes[i].weight;
	}
	int size = usedchar;  
	treenode* root = buildHuffmanTree(arr, freq, size); 
	initcode(root);
}
void initcode(treenode* root){
	int i,j;
	for(i=0;i<256;i++){
		arrofcodes[i][0]=-1;
	}
	int arr1[100], top = 0;
	savecodes(root, arr1, top);
}
void savecodes(treenode* root, int arr[], int top){ 
	if (root->left) { 
		arr[top] = 0; 
		savecodes(root->left, arr, top + 1); 
	}  
	if (root->right){ 
		arr[top] = 1; 
		savecodes(root->right, arr, top + 1); 
	} 
	if (!(root->left) && !(root->right)) {  
		int i; 
		for (i = 0; i < top; ++i){ 
			arrofcodes[root->data][i]=arr[i];
		}
		arrofcodes[root->data][i]=-1; 	
	} 
}
void encodealphabet(FILE *fout, int character) {
    	int stackidx = 0,i;
    	for(i=0;i<100;i++){
		if(arrofcodes[character][i]!=-1){
			stack[i]=arrofcodes[character][i];
		}
		else{
			break;
		}
    	}
    	while (stackidx<i){
        	writebit(fout, stack[stackidx]);
		stackidx++;
    	}
}
int writebit(FILE *f, int bit) {
    	if (bitsinbuffer == 8) {
        	fwrite(buffer, 1, 1, f);
        	bitsinbuffer = 0;
        	memset(buffer, 0, 1);
    	}
    	if (bit)
        	buffer[0] |= 1 << (7-bitsinbuffer); 
    	bitsinbuffer++;
    	return 0;
}
int flushbuffer(FILE *f) {
    	if (bitsinbuffer) {
        	fwrite(buffer, 1, 1, f);
        	bitsinbuffer = 0;
    	}
    	return 0;
}
treenode* newNode(char data, unsigned freq){ 
	treenode* temp = (treenode*)malloc(sizeof(treenode)); 
	temp->left = temp->right = NULL; 
	temp->data = data; 
	temp->freq = freq;
	return temp; 
} 
tree* createtree(unsigned capacity){ 
	tree* hufftree = (tree*)malloc(sizeof(tree)); 
	hufftree->size = 0; 
	hufftree->array = (treenode**)malloc(capacity * sizeof(treenode*)); 
	return hufftree; 
} 
void minHeapify(tree* hufftree, int idx){ 
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
		minHeapify(hufftree, smallest); 
	} 
} 
treenode* extractMin(tree* hufftree) { 
	treenode* temp = hufftree->array[0]; 
	hufftree->array[0] = hufftree->array[hufftree->size - 1]; 
	--hufftree->size; 
	minHeapify(hufftree, 0); 
	return temp; 
} 
void inserttree(tree* hufftree,treenode* hufftreenode){ 
	hufftree->size++; 
	int i = hufftree->size - 1; 
	while (i && hufftreenode->freq < hufftree->array[(i - 1) / 2]->freq) { 
		hufftree->array[i] = hufftree->array[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 
	hufftree->array[i] = hufftreenode; 
}  
tree* createAndBuildtree(char data[], int freq[], int size){ 
	tree* hufftree = createtree(size); 
	for (int i = 0; i < size; ++i){ 
		hufftree->array[i] = newNode(data[i], freq[i]); 
	}
	hufftree->size = size; 
	return hufftree; 
} 
treenode* buildHuffmanTree(char data[], int freq[], int size) { 
	treenode *left, *right, *top; 
	tree* hufftree = createAndBuildtree(data, freq, size); 
	while (!(hufftree->size==1)) { 
		left = extractMin(hufftree); 
		right = extractMin(hufftree); 
		top = newNode('$', left->freq + right->freq); // '$' is a special value for internal nodes, not used 
		top->left = left; 
		top->right = right; 
		inserttree(hufftree, top); 
	}  
	return extractMin(hufftree); 
} 
				
					

			
		
			
	

