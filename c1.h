#include<stdlib.h>
#include<stdio.h>
typedef struct node{ //Structure to get all characters and their frequency and arrange them in ascending order to build tree
	char index;
	unsigned int weight;
}node;
typedef struct treenode {  //Structure of a huffman tree node
	char data;  
	unsigned freq; 
	struct treenode *left, *right; 
}treenode;  
typedef struct tree { //Structure of tree
	unsigned size; 
	struct treenode** array; 
}tree;
int huffman(char * ipfile, char * opfile); //Main calls this
void findfreq(FILE *f); //Function to find the frequency of chracters in the file
void addleaves();  //Will only send those chracters that have a frequency of more than 0 to add node
void addnode(char index, int weight); //Creates nodes with index and frequency and arranges in ascending order
int writeheader(FILE *f); //Writes header to output file. Used in decompression
void buildhuffmancomptree(); //Will send nodes to tree for building and also save codes into a code array for later use while encoding
void encodealphabet(FILE *fout, int character); //Takes the chracter and writes its code to the output file
int writebit(FILE *f, int bit); //Part of encode alphabet. Writes the code bit by bit

//Tree Functions
treenode* buildHuffmanTree(char data[], int freq[], int size); 
tree* createAndBuildtree(char data[], int freq[], int size);
void inserttree(tree* hufftree,treenode* hufftreenode);
treenode* extractMin(tree* hufftree);
void minHeapify(tree* hufftree, int idx);
tree* createtree(unsigned capacity);
treenode* newNode(char data, unsigned freq);

