
CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

default: account

account: account.o
	${CC} account.o -o account

account.o: account.c
	${CC} ${CFLAGS} account.c

clean:
	rm -f *.o *~

