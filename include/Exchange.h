#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "MatchingEngine.h"
#include "Metrics.h"
#include "OrderBook.h"
#include "OrderQueue.h"
#include "SwapManager.h"
#include "Validator.h"

class Exchange {
public:
    Exchange();
    ~Exchange();

    void start();
    void submit(const Order& order);
    void finish();
    const std::vector<ExecutionReport>& reports() const;
    Metrics& metrics();

private:
    Validator validator_;
    Metrics metrics_;
    SwapManager swapManager_;
    MatchingEngine matchingEngine_;

    std::map<std::string, std::unique_ptr<OrderBook>> books_;
    std::map<std::string, std::unique_ptr<OrderQueue>> queues_;
    std::vector<std::thread> workers_;

    std::vector<ExecutionReport> reports_;
    std::mutex reportMutex_;
    bool finished_ = false;

    void workerLoop(const std::string& instrument);
    void appendReports(const std::vector<ExecutionReport>& batch);
};

#endif
