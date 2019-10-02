CC = gcc
CFLAGS = -Wall -ansi -pedantic -g 
MAIN = stat
OBJS = stat.o
all : $(MAIN)

$(MAIN) : $(OBJS) stat.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

stat.o : stat.c stat.h
	$(CC) $(CFLAGS) -c stat.c

clean :
	rm *.o $(MAIN) core
