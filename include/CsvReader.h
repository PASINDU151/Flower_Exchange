#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <vector>
#include "Order.h"

class CsvReader {
public:
    std::vector<Order> readOrders(const std::string& filePath);
};

#endif
