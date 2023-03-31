# This is a Makefile with recipes to compile the various parts of this
# assignment.

#### Variables ####

# C compiler
CC = gcc

# Do not use position-independent code or executable.
# (Newer Ubuntu GCCs have this enabled by default.)
NOPIE_FLAGS = $(shell (gcc -v 2>&1 | grep -q -- --enable-default-pie) && echo -no-pie) -fno-pie -fno-pic

# Flags to pass to C compiler
CFLAGS = --std=c99 -Wall -Wno-unused-function -g $(NOPIE_FLAGS)

SUPPORT = command_lines.o command_support.o soln_command.a

SOLN_FLAGS = -DSOLN -Dcommand_char_is_space=soln_command_char_is_space -Dcommand_parse=soln_command_parse -Dcommand_show=soln_command_show -Dcommand_free=soln_command_free

ADVERSARIAL_FLAGS = -DADVERSARIAL -Dmalloc=command_adversarial_malloc -Dfree=command_adversarial_free

##### Recipes ####
all: practice.bin demo free show parse adversarial

practice.bin: practice.c
	$(CC) $(CFLAGS) -o $@ $^

demo: command_demo_actual.bin command_demo_expected.bin
free: command_free_test_actual.bin command_free_test_expected.bin
show: command_show_test_actual.bin command_show_test_expected.bin
parse: command_parse_test_actual.bin command_parse_test_expected.bin
adversarial: command_adversarial_test_actual.bin command_adversarial_test_expected.bin

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
command_adversarial.o: command.c command.h
	$(CC) $(CFLAGS) $(ADVERSARIAL_FLAGS) -c $< -o command_adversarial.o

command_%_actual.bin: command_%.c $(SUPPORT) command.o 
	$(CC) $(CFLAGS) -o $@ $^
command_%_expected.bin: command_%.c $(SUPPORT)
	$(CC) $(CFLAGS) $(SOLN_FLAGS) -o $@ $^

command_adversarial_test_actual.bin: command_parse_test.c $(SUPPORT) command_adversarial.o 
	$(CC) $(CFLAGS) $(ADVERSARIAL_FLAGS) -o $@ $^
command_adversarial_test_expected.bin: command_parse_test.c $(SUPPORT) command_adversarial.o 
	$(CC) $(CFLAGS) $(SOLN_FLAGS) $(ADVERSARIAL_FLAGS) -o $@ $^

# Clean up executables
clean:
	rm -f *.o *.bin
	rm -rf *.dSYM

# Tests
test-show: show
	valgrind ./command_show_test_actual.bin
test-free: free
	valgrind ./command_free_test_actual.bin
test-demo: demo
	valgrind ./command_demo_actual.bin
test-parse: parse
	valgrind ./command_parse_test_actual.bin
test-adversarial: adversarial
	valgrind ./command_adversarial_test_actual.bin
test: test-show test-free test-demo test-parse test-adversarial

.PHONY: all clean demo free show parse adversarial test-demo test-free test-show test-parse test-adversarial test

