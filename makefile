TARGET = main
DEPENDENCY = 380067_380415_ED2_T01
CC = gcc
CFLAGS = -ansi

default: sistema

sistema: $(TARGET).o $(DEPENDENCY).o
	$(CC) $(CFLAGS) -o sistema $(DEPENDENCY).o $(TARGET).o

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

$(DEPENDENCY).o: $(DEPENDENCY).c $(DEPENDENCY).h
	$(CC) $(CFLAGS) -c $(DEPENDENCY).c

clean:
	$(RM) *.o *~

uninstall:
	$(RM) *.o *.idx *.dat sistema *.txt *.out *.in *~
