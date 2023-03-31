# This is a Makefile with recipes to compile the practice programming
# parts of this lab.

#### Variables ####

#### Variables ####
EXAMPLES = main0.bin main1.bin main2.bin main3.bin main4.bin main5.bin main6.bin main7.bin main8.bin main9.bin mystery0.bin mystery1.bin
BUGS = fibloop.bin fibloop2.bin matvec.bin search.bin strings.bin strings2.bin strings3.bin
EXECUTABLES = $(EXAMPLES) $(BUGS) practice.bin

# C compiler
CC = gcc

# Flags to pass to C compiler
CFLAGS = -Wall -g --std=c99

##### Recipes ####
all: predictions practice.bin bugs

# Output prediction mysteries
predictions: $(EXAMPLES)
%.o: %.c
	$(CC) $(CFLAGS) -c $<
mystery%.bin: mystery%.o hexdump.o
	$(CC) $(CFLAGS) -o $@ $< hexdump.o
main%.bin: main%.o hexdump.o
	$(CC) $(CFLAGS) -o $@ $< hexdump.o

# Debugging practice
bugs: $(BUGS)
%.bin: %.o
	$(CC) $(CFLAGS) -o $@ $*.o

# Pointer programming
practice.bin: practice.c
	$(CC) $(CFLAGS) -o $@ practice.c

# Clean up executables
clean:
	rm -f $(EXECUTABLES) *.bin
	rm -rf *.o *.dSYM
