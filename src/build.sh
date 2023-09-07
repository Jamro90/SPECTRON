#!/bin/bash
set -xe

#x86_64-w64-mingw32-gcc -Iraylib_win64/include/ -o rayWindows.exe rayWindows.c -Lraylib_win64/lib/ -lraylib -lwinmm -lgdi32 -static

gcc test_ray.c -o test_ray -lraylib -ldl -lrt -lm -pthread -Wall -std=c99 -lX11

