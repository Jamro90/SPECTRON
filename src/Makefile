CC=clang
PROG = spectron
SRC=main.c gui_maker.c calc.c
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-s -Wall -std=c99 -ldl -lrt -lm -lraylib -lpthread -g -static-libgcc 
WINFLAGS=-I raylib/src -L raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc
# Declaring rules not target files
.PHONY: all win32 win64 clear 

# Standard compilation rules
all: $(PROG)

# finale compilation from object files
$(PROG): $(OBJ)
	@$(CC) -o $@ $^ $(LFLAGS) 
	@echo "Program Compiled!"

# precompilation for creating objects files
%.o: %.c
	@$(CC) -c `pkg-config --cflags plplot` -o $@ $< $(LFLAGS) `pkg-config --libs plplot`
	@echo "Objects Compiled!"

# Cleaning rule for program compilation & objects files
clean:
	@rm -rf *.o $(PROG) $(PROG)*.exe
	@echo "Cleaning Done!"

win32: $(SRC)
	@i686-w64-mingw32-gcc `pkg-config --cflags plplot` $^ $(WINFLAGS) -o $(PROG)_32.exe `pkg-config --libs plplot`

win64: $(SRC)
	@x86_64-w64-mingw32-gcc `pkg-config --cflags plplot` $^ $(WINFLAGS) -o $(PROG)_64.exe
