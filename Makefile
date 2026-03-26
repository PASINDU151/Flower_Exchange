CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude
SRC = $(wildcard src/*.cpp)
OUT = build/flower_exchange

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT) data/orders.csv data/execution_rep.csv

clean:
	rm -rf build
