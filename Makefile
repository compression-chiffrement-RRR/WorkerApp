#
# Compiler flags
#
CC     = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra

#
# Project files
#
SRCS = $(wildcard **/*.c) 
SRCS += $(wildcard **/*.cpp)
EXE  = workerapp

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGCFLAGS = -g -O0 -DDEBUG -DAES_DEBUG

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELCFLAGS = -O3 -DNDEBUG -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-protector-all -fcf-protection

.PHONY: all clean debug prepare release

# Default build
all: prepare release

#
# Debug
#
debug: prepare $(DBGEXE)

$(DBGEXE): $(SRCS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

#
# Release
#
release: prepare $(RELEXE)

$(RELEXE): $(SRCS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

#
# Other rules
#

remake: clean all

prepare: 
	@mkdir -p $(DBGDIR) $(RELDIR)

clean: 
	rm -f $(RELEXE) $(DBGEXE) $(DBGOBJS)