#include "OrderBook.h"
#include "Utils.h"
#include <algorithm>

OrderBook::OrderBook(const std::string& instrument, Metrics& metrics, SwapManager& swapManager)
    : instrument_(instrument), metrics_(metrics), swapManager_(swapManager) {}

std::string OrderBook::instrument() const {
    return instrument_;
}

ExecutionReport OrderBook::makeReport(const Order& order,
                                      ExecStatus status,
                                      int quantity,
                                      double price,
                                      const std::string& reason) const {
    ExecutionReport r;
    r.clientOrderId = order.clientOrderId;
    r.orderId = order.systemOrderId;
    r.instrument = order.instrument;
    r.side = Utils::sideToInt(order.side);
    r.price = price;
    r.quantity = quantity;
    r.status = static_cast<int>(status);
    r.reason = reason;
    r.transactionTime = Utils::currentTimestamp();
    return r;
}

std::size_t OrderBook::totalActiveOrdersUnsafe() const {
    std::size_t total = 0;
    for (const auto& [p, q] : buyLevels_) total += q.size();
    for (const auto& [p, q] : sellLevels_) total += q.size();
    return total;
}

void OrderBook::addRemainingToBookUnsafe(const Order& order) {
    if (order.side == Side::Buy) {
        buyLevels_[order.price].push_back(order);
    } else {
        sellLevels_[order.price].push_back(order);
    }
    metrics_.addEstimatedMemory(sizeof(Order));
}

void OrderBook::maybeSpillToDiskUnsafe() {
    std::size_t total = totalActiveOrdersUnsafe();
    if (total <= MAX_IN_MEMORY_ORDERS) return;

    std::vector<Order> spilled;

    while (total > MAX_IN_MEMORY_ORDERS && !buyLevels_.empty()) {
        auto it = std::prev(buyLevels_.end());
        while (!it->second.empty() && total > MAX_IN_MEMORY_ORDERS) {
            spilled.push_back(it->second.back());
            it->second.pop_back();
            metrics_.removeEstimatedMemory(sizeof(Order));
            --total;
        }
        if (it->second.empty()) buyLevels_.erase(it);
        if (!spilled.empty()) {
            swapManager_.spillOrders(instrument_, "buy", spilled);
            spilled.clear();
        }
    }

    while (total > MAX_IN_MEMORY_ORDERS && !sellLevels_.empty()) {
        auto it = std::prev(sellLevels_.end());
        while (!it->second.empty() && total > MAX_IN_MEMORY_ORDERS) {
            spilled.push_back(it->second.back());
            it->second.pop_back();
            metrics_.removeEstimatedMemory(sizeof(Order));
            --total;
        }
        if (it->second.empty()) sellLevels_.erase(it);
        if (!spilled.empty()) {
            swapManager_.spillOrders(instrument_, "sell", spilled);
            spilled.clear();
        }
    }
}

std::vector<ExecutionReport> OrderBook::processOrder(const Order& incoming) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<ExecutionReport> reports;

    Order working = incoming;
    const int originalQty = working.quantity;
    int executedQty = 0;

    if (working.side == Side::Buy) {
        while (working.quantity > 0 && !sellLevels_.empty()) {
            auto bestSellIt = sellLevels_.begin();
            double bestSellPrice = bestSellIt->first;

            if (bestSellPrice > working.price) break;

            auto& levelQueue = bestSellIt->second;
            Order& resting = levelQueue.front();

            int matchQty = std::min(working.quantity, resting.quantity);
            working.quantity -= matchQty;
            resting.quantity -= matchQty;
            executedQty += matchQty;

            ExecStatus status = (working.quantity == 0 && executedQty == originalQty)
                ? ExecStatus::Fill : ExecStatus::PFill;

            reports.push_back(makeReport(incoming, status, matchQty, bestSellPrice));

            if (resting.quantity == 0) {
                levelQueue.pop_front();
                metrics_.removeEstimatedMemory(sizeof(Order));
            }

            if (levelQueue.empty()) {
                sellLevels_.erase(bestSellIt);
            }
        }
    } else {
        while (working.quantity > 0 && !buyLevels_.empty()) {
            auto bestBuyIt = buyLevels_.begin();
            double bestBuyPrice = bestBuyIt->first;

            if (bestBuyPrice < working.price) break;

            auto& levelQueue = bestBuyIt->second;
            Order& resting = levelQueue.front();

            int matchQty = std::min(working.quantity, resting.quantity);
            working.quantity -= matchQty;
            resting.quantity -= matchQty;
            executedQty += matchQty;

            ExecStatus status = (working.quantity == 0 && executedQty == originalQty)
                ? ExecStatus::Fill : ExecStatus::PFill;

            reports.push_back(makeReport(incoming, status, matchQty, bestBuyPrice));

            if (resting.quantity == 0) {
                levelQueue.pop_front();
                metrics_.removeEstimatedMemory(sizeof(Order));
            }

            if (levelQueue.empty()) {
                buyLevels_.erase(bestBuyIt);
            }
        }
    }

    if (executedQty == 0) {
        addRemainingToBookUnsafe(working);
        reports.push_back(makeReport(working, ExecStatus::New, working.quantity, working.price));
    } else if (working.quantity > 0) {
        addRemainingToBookUnsafe(working);
        reports.push_back(makeReport(working, ExecStatus::PFill, working.quantity, working.price,
                                     "Remaining quantity added to book"));
    }

    maybeSpillToDiskUnsafe();
    return reports;
}
