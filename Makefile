SHELL := /bin/sh

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude

SRC := $(wildcard src/*.cpp)

# Cross-platform bits
# - On Windows, set OS=Windows_NT in most environments.
# - Use .exe suffix and Windows-friendly mkdir/rm/run commands.
ifeq ($(OS),Windows_NT)
	EXE := .exe
	MKDIR_P := if not exist build mkdir build
	RM_RF := rmdir /s /q build
	RUN_PREFIX :=
else
	EXE :=
	MKDIR_P := mkdir -p build
	RM_RF := rm -rf build
	RUN_PREFIX := ./
endif

OUT := build/flower_exchange$(EXE)

all: $(OUT)

$(OUT): $(SRC)
	$(MKDIR_P)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	$(RUN_PREFIX)$(OUT) data/orders.csv data/execution_rep.csv

clean:
	$(RM_RF)
