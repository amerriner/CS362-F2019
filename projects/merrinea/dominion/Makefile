CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.c rngs.h
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.c dominion.h rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testRandom1: randomtestcard1.c dominion.o rngs.o
	gcc -o testRandom1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

testRandom2: randomtestcard2.c dominion.o rngs.o
	gcc -o testRandom2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

testRandom3: randomtestcard3.c dominion.o rngs.o
	gcc -o testRandom3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)

randomtestresults: testRandom1 testRandom2 testRandom3 
	./testRandom1 >> randomtestresults.out
	./testRandom2 >> randomtestresults.out
	./testRandom3 >> randomtestresults.out
	gcov -b -f dominion.c >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.c interface.h
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
