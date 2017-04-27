CC = gcc
CFLAGS = -Wall -std=c11 -g `xml2-config --cflags`
LIBS = `xml2-config --libs`
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
