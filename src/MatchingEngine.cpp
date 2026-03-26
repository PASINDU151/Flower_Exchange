#include "MatchingEngine.h"

std::vector<ExecutionReport> MatchingEngine::process(OrderBook& book, const Order& order) {
    return book.processOrder(order);
}
