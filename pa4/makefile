COMPILER = gcc
CFLAGS  = -ansi -pedantic -Wall -m32
all: y86emul

debug:
	make DEBUG=TRUE

y86emul:  Stack.o y86emul.o Instructions.o
	$(COMPILER) $(CFLAGS) -o y86emul y86emul.o Stack.o Instructions.o
Stack.o: Stack.h Stack.c
	$(COMPILER) $(CFLAGS) -c Stack.h Stack.c
y86emul.o: y86emul.h y86emul.c 
	$(COMPILER) $(CFLAGS) -c y86emul.h y86emul.c
Instructions.o: Instructions.h Instructions.c
	$(COMPILER) $(CFLAGS) -c Instructions.h Instructions.c



ifeq ($(DEBUG), TRUE)
CC += -g
endif

clean:
	rm -f y86emul
	rm -f *.o
	rm -f *.h.gch
	rm -f *.txt
