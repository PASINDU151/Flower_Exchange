#include <iostream>
#include <stdexcept>
#include "CsvReader.h"
#include "CsvWriter.h"
#include "Exchange.h"

int main(int argc, char* argv[]) {
    try {
        std::string inputFile = "data/orders.csv";
        std::string outputFile = "data/execution_rep.csv";

        if (argc >= 2) inputFile = argv[1];
        if (argc >= 3) outputFile = argv[2];

        CsvReader reader;
        CsvWriter writer;
        Exchange exchange;

        std::cout << "Reading orders from: " << inputFile << "\n";
        auto orders = reader.readOrders(inputFile);

        exchange.start();
        for (const auto& order : orders) {
            exchange.submit(order);
        }
        exchange.finish();

        writer.writeReports(outputFile, exchange.reports());

        std::cout << "Execution report written to: " << outputFile << "\n";
        std::cout << "Performance metrics are printed in the terminal after execution.\n";
        exchange.metrics().printSummary();

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}
