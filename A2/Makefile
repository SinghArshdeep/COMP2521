# Makefile for Ass2

CC = gcc
CFLAGS =  -Wall -std=c11 -Werror -g
AR = ar
objs = Graph.o CentralityMeasures.o PQ.o Dijkstra.o GraphRead.o 

all :  testDijkstra testCentralityMeasures testLanceWilliamsHAC

$(objs) : %.o : %.c

GraphLib.a : $(objs)
	$(AR) rcs $@ $^

BSTree.o : BSTree.c BSTree.h 
	$(CC)  -c   BSTree.c  


clean :
	rm -f *.o testCentralityMeasures testDijkstra  testLanceWilliamsHAC GraphLib.a
	
	
