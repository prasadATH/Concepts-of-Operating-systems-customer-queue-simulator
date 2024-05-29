C = gcc
CFLAGS =-g -Wall -pedantic -ansi -w
OBJ = main.o c_queue.o readFile.o mutexList.o tellerProcess.o
LDFLAGS = -pthread
EXEC = execute

$(EXEC) : $(OBJ)
	        $(CC) $(LDFLAGS) $(OBJ) -o $(EXEC)

main.o : main.c readFile.h c_queue.h tellerProcess.h
	        $(CC) -c main.c $(CFLAGS)
mutexList.o : mutexList.c mutexList.h
	        $(CC) -c mutexList.c $(CFLAGS)
c_queue.o : c_queue.c c_queue.h
	        $(CC) -c c_queue.c $(CFLAGS)
tellerProcess.o : tellerProcess.c tellerProcess.h c_queue.h
	        $(CC) -c tellerProcess.c $(CFLAGS)
readFile.o : readFile.c readFile.h c_queue.h 
	        $(CC) -c readFile.c $(CFLAGS)
clean:
	        rm -f $(EXEC) $(OBJ)

