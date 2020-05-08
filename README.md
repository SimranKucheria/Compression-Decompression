# Compression-Decompression
Using 2 techniques - Huffman and LZW 

Download all files to a folder
>make
then run the following command

>./program <option> <filename> <destination filename>

The options are c1,c2 for Huffman,LZW Compression respectively and uc1, uc2 for Huffman and LZW decompression.
For compression filename should be a txt file and destination file a dat file(Need not exist. Will be created.
For destination filename to be dat file and destination filename to be txt file.(Will be created)
for eg:-
>./program c1 1.txt 2.dat
to decompress 2.dat run the following command

>./program uc1 2.dat 3.txt

1.txt and 3.txt will be identical. and 2.dat will be significantly smaller than 1.txt
