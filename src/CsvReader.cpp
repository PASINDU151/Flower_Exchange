#include "CsvReader.h"
#include "Utils.h"
#include <cctype>
#include <fstream>
#include <stdexcept>

std::vector<Order> CsvReader::readOrders(const std::string& filePath) {
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open input file: " + filePath);
    }

    std::vector<Order> orders;
    std::string line;
    std::uint64_t seq = 1;

    bool firstLine = true;
    while (std::getline(fin, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;

        auto cols = Utils::splitCsvLine(line);

        if (firstLine) {
            firstLine = false;
            if (!cols.empty()) {
                std::string c0 = Utils::trim(cols[0]);
                std::string c1 = cols.size() > 1 ? Utils::trim(cols[1]) : "";
                std::string c2 = cols.size() > 2 ? Utils::trim(cols[2]) : "";

                for (auto& ch : c0) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
                for (auto& ch : c1) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
                for (auto& ch : c2) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

                if (c0 == "cl. ord.id" ||
                    c0 == "client order id" ||
                    c0.find("client") != std::string::npos ||
                    (c1 == "instrument" && c2 == "side")) {
                    continue;
                }
            }
        }

        Order order;
        if (cols.size() > 0) order.clientOrderId = cols[0];
        if (cols.size() > 1) order.instrument = cols[1];
        if (cols.size() > 2) {
            const std::string& sideStr = cols[2];

            if (sideStr == "1") {
                order.side = Side::Buy;
            } else if (sideStr == "2") {
                order.side = Side::Sell;
            } else {
                order.side = Side::Unknown;
            }
        }
        if (cols.size() > 3) {
            try {
                order.quantity = std::stoi(cols[3]);
            } catch (...) {
                order.quantity = 0;
            }
        }
        if (cols.size() > 4) {
            try {
                order.price = std::stod(cols[4]);
            } catch (...) {
                order.price = 0.0;
            }
        }


        order.sequence = seq++;
        order.systemOrderId = Utils::generateSystemOrderId();
        order.transactionTime = Utils::currentTimestamp();

        orders.push_back(order);
    }

    return orders;
}
