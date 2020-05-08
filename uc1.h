#include<stdio.h>
#include "c1.h" //For defintion of tree node etc
int dehuffman(char* ipfile,char *opfile); //Main calls this
int readheader(FILE *f);     //Reads header encoded during compression
void decodebit(FILE *fin, FILE *fout, treenode* root); //Calls readbit and traverses tree
void buildhuffmancomptree1(FILE *fin, FILE * fout); //Creates tree using header
int readbit(FILE *f); //Reads single bit

//Tree functions
treenode* buildHuffmanTree1(char data[], int freq[], int size);
tree* createAndBuildtree1(char data[], int freq[], int size);
void inserttree1(tree* hufftree,treenode* hufftreenode);
treenode* extractMin1(tree* hufftree);
void minHeapify1(tree* hufftree, int idx);
tree* createtree1(unsigned capacity);
treenode* newNode1(char data, unsigned freq);
 
