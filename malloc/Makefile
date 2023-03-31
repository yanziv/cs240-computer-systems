# CS 240 Malloc
CC = gcc

# Compiler Option Flags
#
# To build executables that will measure profiling information when
# run, add the -pg option. While this is useful to record information
# that can help figure out how to improve performance, it actually
# slows down your program, so DISABLE this before testing for best
# performance!
CFLAGS = -Wall -Wno-unused-function -Wno-unused-variable -g
OPTFLAGS = -O3 -DNDEBUG

OBJS = memlib.o fsecs.o fcyc.o clock.o ftimer.o

all: mdriver.bin mdriver.opt.bin

mdriver.bin: mm.o mdriver.o $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $^ -o $@
mdriver.opt.bin: mm.opt.o mdriver.o $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $^ -o $@

mdriver-realloc.bin: mm.o mdriver-realloc.o $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $^ -o $@
mdriver-realloc.opt.bin: mm.opt.o mdriver-realloc.o $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $^ -o $@

mm.o: mm.c | mm.h memlib.h Makefile
	$(CC) $(CFLAGS) -std=c99 -c $< -o $@
mm.opt.o: mm.c | mm.h memlib.h Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) -std=c99 -c $< -o $@

mdriver.o: mdriver.c | fsecs.h fcyc.h clock.h memlib.h config.h mm.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
mdriver-realloc.o: mdriver-realloc.c | fsecs.h fcyc.h clock.h memlib.h config.h mm.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
memlib.o: memlib.c | memlib.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
fsecs.o: fsecs.c | fsecs.h config.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
fcyc.o: fcyc.c | fcyc.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
ftimer.o: ftimer.c | ftimer.h config.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<
clock.o: clock.c | clock.h
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	rm -f *.o *.bin

.PHONY: all clean
