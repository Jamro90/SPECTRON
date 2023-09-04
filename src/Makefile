CC=clang
PROG = spectron
SRC=main.c gui_maker.c calc.c
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-Wextra -Iraylib_binaries/raylib/src -Lraylib_binaries/raylib/src -Wall -std=c99 -ldl -lrt -lraylib -lpthread -g -lm

# Declaring rules not target files
.PHONY: all win32 win64 clean

# Standard compilation rules
all: $(PROG) win32 win64

dist: $(PROG) win32 win64
	@rm *.o

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
	@i686-w64-mingw32-gcc -Iraylib_binaries/raylib_win32/include/ -o $(PROG)_32.exe main.c gui_maker.c calc.c foo.c -Lraylib_binaries/raylib_win32/lib/ -lraylib -lgdi32 -lwinmm -static -static-libgcc


win64: $(SRC)
	@x86_64-w64-mingw32-gcc -Iraylib_binaries/raylib_win64/include/ -o $(PROG)_64.exe main.c gui_maker.c calc.c foo.c -Lraylib_binaries/raylib_win64/lib/ -lraylib -lgdi32 -lwinmm -static -static-libgcc


