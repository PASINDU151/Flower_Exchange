# Flower_Exchange

A C++ flower exchange simulation with:
- one order book per instrument
- one queue per instrument
- one worker thread per instrument
- validation
- price-time priority matching
- execution report CSV generation
- performance metrics for runtime, estimated memory, and secondary storage usage

## Folder layout

- `include/` - header files
- `src/` - implementation files
- `data/orders.csv` - sample input CSV
- `data/execution_rep.csv` - output CSV appears here by default
- `swap/` - spill files appear here if in-memory threshold is exceeded
- `build/` - compiled executable appears here

## Build

From the project root:

```bash
make
```

## Run

Default run:

```bash
make run
```

Or explicitly:

```bash
./build/flower_exchange data/orders.csv data/execution_rep.csv
```

On Windows with MinGW:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude src/*.cpp -o build/flower_exchange.exe
build\flower_exchange.exe data/orders.csv data/execution_rep.csv
```

## Where output appears

- The execution report CSV is written to:

```text
data/execution_rep.csv
```

unless you pass a different output path as the second command-line argument.

## Where performance metrics appear

Performance metrics are printed in the terminal/console after execution, including:
- total runtime in milliseconds
- number of orders read
- number of orders processed
- number of reports generated
- estimated current RAM
- estimated peak RAM
- secondary storage used
- per-book active durations

## Notes

- The engine keeps orders in memory first.
- If active orders exceed the in-memory threshold, lower-priority levels are spilled to files inside `swap/`.
- The included sample CSV contains both valid and invalid orders so you can see matching and rejection behavior.
