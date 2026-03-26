#ifndef EXECUTION_REPORT_H
#define EXECUTION_REPORT_H

#include <string>

enum class ExecStatus {
    New = 0,
    Rejected = 1,
    Fill = 2,
    PFill = 3
};

struct ExecutionReport {
    std::string clientOrderId;
    std::string orderId;
    std::string instrument;
    int side = 0;
    double price = 0.0;
    int quantity = 0;
    int status = 0;
    std::string reason;
    std::string transactionTime;
};

#endif
