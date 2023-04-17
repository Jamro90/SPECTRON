CC=gcc
PROG = spectron
SRC=main.c
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-s -Wall -std=c99 -ldl -lrt -lm -lraylib -lpthread -lX11
# Declaring rules not target files
.PHONY: all clear 

# Standard compilation rules
all: $(PROG)

# finale compilation from object files
$(PROG): $(OBJ)
	@$(CC) -o $@ $^ $(LFLAGS)
	@echo "Program Compiled!"

# precompilation for creating objects files
%.o: %.c
	@$(CC) -c -o $@ $< $(LFLAGS)
	@echo "Objects Compiled!"

# Cleaning rule for program compilation & objects files
clean:
	@rm -rf *.o $(PROG) $(PROG).exe
	@echo "Cleaning Done!"
