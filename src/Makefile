CC=clang
PROG = spectron
SRC=main.c gui_maker.c calc.c 
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-Wextra -Wpedantic -Iraylib_binaries/raylib/src -Lraylib_binaries/raylib/src -Wall -std=c99 -ldl -lrt -lraylib -lpthread -g -lm

# Declaring rules not target files
.PHONY: all win32 win64 clean

# Standard compilation rules
$(PROG): $(OBJ)
	@$(CC) -o $@ $^ $(LFLAGS) 
	@printf "\x1b[32mProgram spectron Compiled!\x1b[0m\n"

# precompilation for creating objects files
%.o: %.c
	@$(CC) -c -o $@ $< $(LFLAGS) 
	@printf "\x1b[33mObjects $@ Compiled!\x1b[0m\n"

# Cleaning rule for program compilation & objects files
clean:
	@rm -rf *.o $(PROG) $(PROG)*.exe
	@printf "\x1b[31mCleaning Done!\x1b[0m\n"

# Windows 32-bits compilation
win32: $(SRC)
	@i686-w64-mingw32-gcc -Iraylib_binaries/raylib_win32/include/ -o spectron_32.exe main.c gui_maker.c calc.c  -Lraylib_binaries/raylib_win32/lib/ -lraylib -lgdi32 -lwinmm -static -static-libgcc
	@printf "\x1b[32mWindows spectron_32.exe Compiled!\x1b[0m\n"

# Windows 64-bits compilation
win64: $(SRC)
	@x86_64-w64-mingw32-gcc -Iraylib_binaries/raylib_win64/include/ -o spectron_64.exe main.c gui_maker.c calc.c  -Lraylib_binaries/raylib_win64/lib/ -lraylib -lgdi32 -lwinmm -static -static-libgcc
	@printf "\x1b[32mWindows spectron_64.exe Compiled!\x1b[0m\n"
