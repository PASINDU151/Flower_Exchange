# 🌸 Flower Exchange

A high-performance C++ flower exchange simulation system featuring:

- Multi-instrument order books  
- Per-instrument parallel processing  
- Price-time priority matching  
- Order validation  
- Execution report generation (CSV)  
- Performance metrics (runtime, memory, storage)

---

## 📁 Project Structure

```
Flower_Exchange/
├── include/
├── src/
├── data/
│   ├── orders.csv
│   └── execution_rep.csv
├── swap/
├── build/
└── Makefile
```

---

## 📥 Input & Output

### Where to put input file
Place your input CSV inside the `data/` folder.

Example:
```
data/Example5_orders.csv
```

---

### Where output will appear
The output CSV will be generated in the `data/` folder by default:

```
data/execution_rep.csv
```

Or in your custom path if provided.

---

## ⚙️ Requirements

- C++17 compatible compiler (`g++`)
- `make`

---

## 🚀 Build & Run (Using Make)

### 🟢 Note
- Works directly on Linux/macOS
- On Windows, use Git Bash, MSYS2, or WSL

### Build
```bash
make
```

### Run (default)
```bash
make run
```

### Run with custom input/output
```bash
make run INPUT=data/Example5_orders.csv OUTPUT=data/Example5_rep.csv
```

---

## 🖥️ Manual Compilation (Without Make)

### Windows
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude src/*.cpp -o build/flower_exchange.exe
build\flower_exchange.exe data/orders.csv data/execution_rep.csv
```

### Linux / macOS
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pthread -Iinclude src/*.cpp -o build/flower_exchange
./build/flower_exchange data/orders.csv data/execution_rep.csv
```

---

## 📊 Output

### Execution Report
Generated CSV includes:

- Order ID  
- Client Order ID  
- Instrument  
- Side  
- Execution Status (New / Rejected / Fill / PFill)  
- Quantity  
- Price  
- Reason  
- Timestamp  

---

### Performance Metrics

Printed in terminal after execution:

- Total runtime  
- Orders read  
- Orders processed  
- Reports generated  
- Estimated memory usage  
- Peak memory usage  
- Secondary storage usage  
- Per-instrument processing time  

---

## ⚠️ Notes

- Orders are processed in memory first  
- If memory threshold is exceeded, data spills into the `swap/` directory  
- Sample CSV includes both valid and invalid orders for testing  

---

## 💡 Quick Start

```bash
make
make run
```

---

## 🧠 Matching Logic

- Buy orders → higher price priority  
- Sell orders → lower price priority  
- Same price → FIFO (time priority)  
