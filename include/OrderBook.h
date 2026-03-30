#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <deque>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include "ExecutionReport.h"
#include "Metrics.h"
#include "Order.h"
#include "SwapManager.h"

class OrderBook {
public:
    OrderBook(const std::string& instrument, Metrics& metrics, SwapManager& swapManager);

    std::vector<ExecutionReport> processOrder(const Order& incoming);
    std::string instrument() const;

private:
    using PriceLevel = std::deque<Order>;

    std::string instrument_;
    Metrics& metrics_;
    SwapManager& swapManager_;

    std::map<double, PriceLevel, std::greater<double>> buyLevels_; // highest first
    std::map<double, PriceLevel> sellLevels_;                      // lowest first

    mutable std::mutex mutex_;

    static constexpr std::size_t MAX_IN_MEMORY_ORDERS = 50000;

    ExecutionReport makeReport(const Order& order,
                               ExecStatus status,
                               int quantity,
                               double price,
                               const std::string& reason = "") const;

    std::size_t totalActiveOrdersUnsafe() const;
    void addRemainingToBookUnsafe(const Order& order);
    void maybeSpillToDiskUnsafe();
};

#endif
