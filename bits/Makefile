#
# Makefile that builds btest.bin and other helper programs
# 
CC = gcc
# Disable all optimizations, since newer GCCs exploit undefined
# overflow more aggressively.
CFLAGS = -O0 -g
LIBS = -lm
OBJS = btest.o decl.o tests.o

all: btest.bin fshow.bin ishow.bin

%.o: %.c btest.h bits.h
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

btest.bin: bits.c $(OBJS)
	$(CC) -Wall $(CFLAGS) $(LIBS) -o $@ $(OBJS) bits.c

fshow.bin: fshow.c
	$(CC) $(CFLAGS) -o $@ fshow.c

ishow.bin: ishow.c
	$(CC) $(CFLAGS) -o $@ ishow.c

# Forces a recompile. Used by the driver program. 
btestexplicit: clean
	make btest.bin

clean:
	rm -rf *.o *.bin *~ *.dSYM

.PHONY: all btestexplicit clean
