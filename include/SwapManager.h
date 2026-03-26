#ifndef SWAP_MANAGER_H
#define SWAP_MANAGER_H

#include <string>
#include <vector>
#include "Metrics.h"
#include "Order.h"

class SwapManager {
public:
    explicit SwapManager(Metrics& metrics, const std::string& swapDir = "swap");

    void spillOrders(const std::string& instrument,
                     const std::string& sideLabel,
                     const std::vector<Order>& orders);

private:
    Metrics& metrics_;
    std::string swapDir_;
};

#endif
