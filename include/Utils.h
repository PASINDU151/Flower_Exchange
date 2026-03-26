#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "Order.h"
#include "ExecutionReport.h"

namespace Utils {
    std::string trim(const std::string& s);
    std::vector<std::string> splitCsvLine(const std::string& line);
    std::string currentTimestamp();
    std::string generateSystemOrderId();
    Side intToSide(int side);
    int sideToInt(Side side);
    std::string sideToString(Side side);
    std::string execStatusToString(ExecStatus status);
    bool isValidInstrument(const std::string& instrument);
}

#endif
