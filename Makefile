CC=gcc

program: c1.o c2.o uc1.o uc2.o main.o 
	$(CC) -o program c1.o c2.o uc1.o uc2.o main.o

c1.o: c1.c
	$(CC) -c c1.c

c2.o: c2.c
	$(CC) -c c2.c

uc1.o: uc1.c
	$(CC) -c uc1.c

uc2.o: uc2.c
	$(CC) -c uc2.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f *.o c1 c2 uc1 uc2 main program
