#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <vector>
#include "ExecutionReport.h"

class CsvWriter {
public:
    void writeReports(const std::string& filePath, const std::vector<ExecutionReport>& reports);
};

#endif
