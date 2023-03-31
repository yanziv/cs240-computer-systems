# Simple Makefile for building introductory C code.
# Comments start with the "#" character.
#
# To compile the "zero.bin" program, run "make" or "make zero.bin".

# The core of a Makefile is a set of *rules*. Below is a rule named
# "zero.bin". A typical Makefile rule has three parts:
#
# - The *target* ("zero.bin", before the colon) is a name for the rule and
#   often also the name of a file to be produced by the rule.
#
# - The *dependencies* ("zero.c ...", following the colon) are files
#   or other rules on which this rule depends.
#
# - The *body* (everything tab-indented under the rule header)
#   indicates actions to take when invoking the rule.
#
# This rule, "zero.bin", depends on the files "zero.c" and
# "zero_test.c", because these are listed after the rule name and
# colon. The rule's action is to compile a binary executable called
# "zero.bin" (indicated by the compiler output option "-o zero.bin")
# with all compiler warnings ("-Wall") and debugging symbols ("-g")
# enabled, using the C source code in the file "zero.c".
zero.bin: zero.c zero_test.c
	gcc -std=c99 -O0 -g zero.c zero_test.c -o zero.bin

# This rule depends on nothing.  Its action is to remove the compiled
# executable "zero.bin".
#
# It is good style to complement rules for automatically generating
# files (such as through compilation) with a rule to clean up the
# automatically generated files.
clean:
	rm -f zero.bin

# Rule to compile and test the "zero.c" code using the "zero_test.c"
# testing harness.
#
# This rule depends on the rule to compile the "zero.bin" executable. If
# the executable needs to be (re-)compiled, that happens first, Then,
# the executable is run with "./zero.bin".
test: zero.bin
	./zero.bin


# More notes...
#
# The "zero.bin" rule is the first rule in this Makefile.  Because it is
# first, it is also the default rule that is used when "make" is run
# without any arguments.  The "zero.bin" rule can also be invoked
# explicitly by running "make zero.bin".
#
# To save time, make skips running the body of a rule if a file with
# the same name as the rule exists and was last modified more recently
# than any of its dependencies. That means that if you run "make zero.bin"
# twice in a row, the first time will run gcc to compile an executable
# named "zero.bin", but the second time will do nothing, because an
# up-to-date "zero.bin" executable already exists.
#
# Unlike the "zero.bin" rule, the "clean" rule does not produce a file
# named "clean". The body of the rule will always run when invoked
# with "make clean". However, if you manually create a file named
# "clean", this rule's body will never run: the "clean" file exists
# and is newer than all of its dependencies (of which there are none).
# While this is typically not a problem for rules like "clean",
# Makefiles support a .PHONY directive that indicates its dependent
# rules should always be run, regardless of file status:
.PHONY: clean test

# For CS 240, we always provide Makefiles for you, so you do not need
# to sweat these details. Complete documentation of make and Makefiles
# is here if you are curious:
# https://www.gnu.org/software/make/manual/html_node/index.html
