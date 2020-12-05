CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

DEBUG_CXXFLAGS = -g3
OPT_CXXFLAGS = -Ofast -ffunction-sections -fdata-sections -Wl,--gc-sections -s

.PHONY: default all opt debug clean
.PRECIOUS: obj/%.o obj/opt/%.o obj/debug/%.o
default: debug

SOURCES = $(wildcard src/day[0-2][0-9].cpp)
OPT_SOLVERS = ${SOURCES:src/%.cpp=bin/opt/%}
DEBUG_SOLVERS = ${SOURCES:src/%.cpp=bin/debug/%}

all: opt debug
opt: ${OPT_SOLVERS}
debug: ${DEBUG_SOLVERS}

clean:
	rm -rf bin

bin:
	mkdir bin

bin/opt bin/debug: | bin
	mkdir $@

bin/opt/%: src/%.cpp | bin/opt
	${CXX} $^ ${CXXFLAGS} ${OPT_CXXFLAGS} -o $@

bin/debug/%: src/%.cpp | bin/debug
	${CXX} $^ ${CXXFLAGS} ${DEBUG_CXXFLAGS} -o $@

