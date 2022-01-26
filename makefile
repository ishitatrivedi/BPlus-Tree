
CC = g++
CFLAG = -g -c

bplustree : BPTNode.o BPTree.o main.o
	$(CC) -o bplustree BPTNode.o BPTree.o main.o

main.o : main.cpp
	$(CC) $(CFLAG) main.cpp

BPTree.o : BPTree.cpp
	$(CC) $(CFLAG) BPTree.cpp

BPTreeNode.o : BPNode.cpp
	$(CC) $(CFLAG) BPNode.cpp

clean: 
	rm -f *.o
	rm -f bplustree
