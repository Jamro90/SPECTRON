CC=clang
PROG = spectron
SRC=main.c gui_maker.c chart_maker.c
# Objects for Linux
OBJ=$(SRC:.c=.o)
# Flags for Linux
LFLAGS=-s -Wall -std=c99 -ldl -lrt -lm -lraylib -lpthread -lX11
WINFLAGS=-I raylib/src -L raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc 
# Declaring rules not target files
.PHONY: all win32 win64 clear 

# Standard compilation rules
all: $(PROG)

# finale compilation from object files
$(PROG): $(OBJ)
	@$(CC) `pkg-config --cflags raylib` -o $@ $^ $(LFLAGS) `pkg-config --libs raylib`
	@echo "Program Compiled!"

# precompilation for creating objects files
%.o: %.c
	@$(CC) -c -o $@ $< $(LFLAGS)
	@echo "Objects Compiled!"

# Cleaning rule for program compilation & objects files
clean:
	@rm -rf *.o $(PROG) $(PROG).exe
	@echo "Cleaning Done!"

win32: $(SRC)
	@i686-w64-mingw32-gcc $^ $(WINFLAGS) -o $(PROG)_32.exe

win64: $(SRC)
	@x86_64-w64-mingw32-gcc $^ $(WINFLAGS) -o $(PROG)_64.exe
