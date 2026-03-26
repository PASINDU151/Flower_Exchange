#include "CsvWriter.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>

void CsvWriter::writeReports(const std::string& filePath, const std::vector<ExecutionReport>& reports) {
    std::ofstream fout(filePath);
    if (!fout.is_open()) {
        throw std::runtime_error("Failed to open output file: " + filePath);
    }

    fout << "Client Order ID,Order ID,Instrument,Side,Price,Quantity,Status,Reason,Transaction Time\n";

    for (const auto& r : reports) {
        fout << r.clientOrderId << ","
             << r.orderId << ","
             << r.instrument << ","
             << r.side << ","
             << std::fixed << std::setprecision(2) << r.price << ","
             << r.quantity << ","
             << r.status << ","
             << "\"" << r.reason << "\"" << ","
             << r.transactionTime
             << "\n";
    }
}
