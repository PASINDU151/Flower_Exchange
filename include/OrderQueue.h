#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include "Order.h"

class OrderQueue {
public:
    void push(const Order& order);
    std::optional<Order> pop();
    void close();
    bool isClosed() const;
    size_t size() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<Order> queue_;
    bool closed_ = false;
};

#endif
