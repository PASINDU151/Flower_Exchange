#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include <vector>
#include "ExecutionReport.h"
#include "Order.h"
#include "OrderBook.h"

class MatchingEngine {
public:
    std::vector<ExecutionReport> process(OrderBook& book, const Order& order);
};

#endif
