#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <cstdint>

enum class Side {
    Buy = 1,
    Sell = 2,
    Unknown = 0
};

struct Order {
    std::string clientOrderId;
    std::string instrument;
    Side side = Side::Unknown;
    double price = 0.0;
    int quantity = 0;

    std::string systemOrderId;
    std::uint64_t sequence = 0;
    std::string transactionTime;
};

#endif
