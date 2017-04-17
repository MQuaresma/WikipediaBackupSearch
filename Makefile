CC = gcc
CFLAGS = -Wall -std=c11 -g -I/usr/include/libxml2
LIBS = -lxml2 -lz -lpthread -licucore -lm
DEPS = interface.h
OBJ = load.o queries.o program.o structManager.o

program: $(OBJ)
	$(CC) $(CFLAGS) -o program $(OBJ) $(LIBS)

program.o: $(DEPS)
	$(CC) -c $(CFLAGS) program.c

structManager.o: structManager.h
	$(CC) -c $(CFLAGS) structManager.c


load.o: structManager.h
	$(CC) -c $(CFLAGS) load.c


queries.o: structManager.h
	$(CC) -c $(CFLAGS) queries.c

clean: 
	rm *.o program 
