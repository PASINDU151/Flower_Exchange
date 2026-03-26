#include "SwapManager.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

SwapManager::SwapManager(Metrics& metrics, const std::string& swapDir)
    : metrics_(metrics), swapDir_(swapDir) {
    std::filesystem::create_directories(swapDir_);
}

void SwapManager::spillOrders(const std::string& instrument,
                              const std::string& sideLabel,
                              const std::vector<Order>& orders) {
    if (orders.empty()) return;

    std::string fileName = swapDir_ + "/" + instrument + "_" + sideLabel + "_spill.csv";
    std::ofstream fout(fileName, std::ios::app);
    if (!fout.is_open()) {
        throw std::runtime_error("Failed to open swap file: " + fileName);
    }

    std::size_t bytes = 0;
    for (const auto& o : orders) {
        std::string line =
            o.clientOrderId + "," +
            o.instrument + "," +
            std::to_string(static_cast<int>(o.side == Side::Buy ? 1 : 2)) + "," +
            std::to_string(o.price) + "," +
            std::to_string(o.quantity) + "," +
            o.systemOrderId + "," +
            std::to_string(o.sequence) + "," +
            o.transactionTime + "\n";

        fout << line;
        bytes += line.size();
    }

    metrics_.addSwapBytes(bytes);
}
