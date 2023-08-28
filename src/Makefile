CC=gcc
PROG = spectron
SRC=main.c gui_maker.c calc.c
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-I raylib/src -L raylib/src -Wall -std=c99 -ldl -lrt -lraylib -lpthread -g -lm -static-libgcc
WINFLAGS32=-I raylib/src_win32 -L raylib/src_win32 -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc
WINFLAGS64=-I raylib/src_win64 -L raylib/src_win64 -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc
# Declaring rules not target files
.PHONY: all win32 win64 clean

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
	@rm -rf *.o $(PROG) $(PROG)*.exe
	@echo "Cleaning Done!"

win32: $(SRC)
	@i686-w64-mingw32-gcc $^ $(WINFLAGS) -o $(PROG)_32.exe 

win64: $(SRC)
	@x86_64-w64-mingw32-gcc $^ $(WINFLAGS) -o $(PROG)_64.exe
