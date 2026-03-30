#include "CsvWriter.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>

static const char* statusToString(int status) {
    switch (status) {
        case 0: return "New";
        case 1: return "Rejected";
        case 2: return "Fill";
        case 3: return "PFill";
        default: return "Unknown";
    }
}

void CsvWriter::writeReports(const std::string& filePath, const std::vector<ExecutionReport>& reports) {
    std::ofstream fout(filePath);
    if (!fout.is_open()) {
        throw std::runtime_error("Failed to open output file: " + filePath);
    }

    fout << "Order ID,Cl. Ord. ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";
    
    for (const auto& r : reports) {
        fout << r.orderId << ","
             << r.clientOrderId << ","
             << r.instrument << ","
             << r.side << ","
             << statusToString(r.status) << ","
             << r.quantity << ","
             << std::fixed << std::setprecision(2) << r.price << ","
             << "\"" << r.reason << "\"" << ","
             << r.transactionTime
             << "\n";
    }
}