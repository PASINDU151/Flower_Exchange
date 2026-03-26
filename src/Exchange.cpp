#include "Exchange.h"
#include "Utils.h"

Exchange::Exchange()
    : swapManager_(metrics_) {
    const std::vector<std::string> instruments = {
        "Rose", "Lavender", "Lotus", "Tulip", "Orchid"
    };

    for (const auto& inst : instruments) {
        books_[inst] = std::make_unique<OrderBook>(inst, metrics_, swapManager_);
        queues_[inst] = std::make_unique<OrderQueue>();
    }
}

Exchange::~Exchange() {
    finish();
}

void Exchange::start() {
    metrics_.start();
    finished_ = false;

    for (const auto& [instrument, queuePtr] : queues_) {
        workers_.emplace_back(&Exchange::workerLoop, this, instrument);
    }
}

void Exchange::submit(const Order& order) {
    metrics_.incrementOrdersRead();

    ValidationResult vr = validator_.validate(order);
    if (!vr.valid) {
        ExecutionReport r;
        r.clientOrderId = order.clientOrderId;
        r.orderId = order.systemOrderId;
        r.instrument = order.instrument;
        r.side = Utils::sideToInt(order.side);
        r.price = order.price;
        r.quantity = order.quantity;
        r.status = static_cast<int>(ExecStatus::Rejected);
        r.reason = vr.reason;
        r.transactionTime = Utils::currentTimestamp();
        appendReports({r});
        metrics_.incrementReportsGenerated();
        metrics_.incrementOrdersProcessed();
        return;
    }

    auto it = queues_.find(order.instrument);
    if (it != queues_.end()) {
        it->second->push(order);
    }
}

void Exchange::finish() {
    if (finished_) return;
    finished_ = true;

    for (auto& [instrument, q] : queues_) {
        if (q) q->close();
    }

    for (auto& t : workers_) {
        if (t.joinable()) t.join();
    }
    workers_.clear();

    metrics_.stop();
}

const std::vector<ExecutionReport>& Exchange::reports() const {
    return reports_;
}

Metrics& Exchange::metrics() {
    return metrics_;
}

void Exchange::appendReports(const std::vector<ExecutionReport>& batch) {
    std::lock_guard<std::mutex> lock(reportMutex_);
    for (const auto& r : batch) {
        reports_.push_back(r);
    }
}

void Exchange::workerLoop(const std::string& instrument) {
    auto& queue = *queues_.at(instrument);
    auto& book = *books_.at(instrument);

    while (true) {
        auto item = queue.pop();
        if (!item.has_value()) {
            if (queue.isClosed()) break;
            continue;
        }

        metrics_.markBookStart(instrument);
        auto batch = matchingEngine_.process(book, item.value());
        metrics_.markBookStop(instrument);

        appendReports(batch);

        metrics_.incrementOrdersProcessed();
        for (size_t i = 0; i < batch.size(); ++i) {
            metrics_.incrementReportsGenerated();
        }
    }
}
