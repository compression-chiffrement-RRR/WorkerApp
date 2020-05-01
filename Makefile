#
# Project files
#
SRCS := $(wildcard src/*.c) 
SRCS += $(wildcard src/*.cpp)

HEADERS := $(wildcard src/*.h) 
HEADERS += $(wildcard src/*.hpp)

#
# Test files
#
TESTS := $(wildcard tests/*.cpp) 

#
# Main build settings
#
CC     := g++
CFLAGS := -std=c++17 -Wall -Werror -Wextra
CLIBS := -lSimpleAmqpClient -lcurl
EXE  := workerapp

#
# Debug build settings
#
DBGDIR := debug
DBGEXE := $(DBGDIR)/$(EXE)
DBGCFLAGS := -g -O0 -DDEBUG #-DAES_DEBUG

#
# Release build settings
#
RELDIR := release
RELEXE := $(RELDIR)/$(EXE)
RELCFLAGS := -O3 -DNDEBUG -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-protector-all -fcf-protection

#
# Tests build settings
#
TESTDIR := tests/bin
TESTEXE := $(TESTDIR)/workerapp-test
TESTCFLAGS := -O3 -g -DDEBUG -lgtest_main -lgtest -lpthread

.PHONY: all clean debug prepare release

# Default build
all: prepare release debug

#
# Debug
#
debug: prepare $(DBGEXE)

$(DBGEXE): $(SRCS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(CLIBS) -o $(DBGEXE) $^

#
# Release
#
release: prepare $(RELEXE)

$(RELEXE): $(SRCS) 
	$(CC) $(CFLAGS) $(RELCFLAGS) $(CLIBS) -o $(RELEXE) $^

#
# Tests
#
test: prepare $(TESTEXE)
	$(TESTEXE)

$(TESTEXE): $(TESTS) $(SRCS) 
	$(CC) $(CFLAGS) $(TESTCFLAGS) $(CLIBS) -o $(TESTEXE) $(filter-out src/main.cpp, $^)

#
# Other rules
#

remake: clean all

prepare: 
	@mkdir -p $(DBGDIR) $(RELDIR) $(TESTDIR)

clean: 
	rm -rf $(DBGDIR) $(RELDIR) $(TESTDIR)