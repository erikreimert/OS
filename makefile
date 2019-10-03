LIB=-lpthread -lrt
CC=gcc
CCPP=g++

all: pcthreads

pcthreads: pcthreads.c
	 $(CC) pcthreads.c -g -o pcthreads $(LIB)

clean:
	/bin/rm -f *.o core pcthreads-c
