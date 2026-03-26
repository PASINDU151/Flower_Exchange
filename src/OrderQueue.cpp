#include "OrderQueue.h"

void OrderQueue::push(const Order& order) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(order);
    }
    cv_.notify_one();
}

std::optional<Order> OrderQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&]() { return closed_ || !queue_.empty(); });

    if (queue_.empty()) {
        return std::nullopt;
    }

    Order order = queue_.front();
    queue_.pop();
    return order;
}

void OrderQueue::close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }
    cv_.notify_all();
}

bool OrderQueue::isClosed() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return closed_;
}

size_t OrderQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}
