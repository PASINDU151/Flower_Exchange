SHELL := /bin/sh

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude

SRC := $(wildcard src/*.cpp)

# Default input/output paths
INPUT ?= data/orders.csv
OUTPUT ?= data/execution_rep.csv

# Windows vs Unix-like handling
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
	$(eval EXTRA_ARGS := $(filter-out run,$(MAKECMDGOALS)))
	$(eval RUN_INPUT := $(if $(word 1,$(EXTRA_ARGS)),$(word 1,$(EXTRA_ARGS)),$(INPUT)))
	$(eval RUN_OUTPUT := $(if $(word 2,$(EXTRA_ARGS)),$(word 2,$(EXTRA_ARGS)),$(OUTPUT)))
	$(RUN_PREFIX)$(OUT) $(RUN_INPUT) $(RUN_OUTPUT)

clean:
	-$(RM_RF)

.PHONY: run clean all

%:
	@: