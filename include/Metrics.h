#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>

class Metrics {
public:
    void start();
    void stop();

    void incrementOrdersRead();
    void incrementOrdersProcessed();
    void incrementReportsGenerated();
    void addEstimatedMemory(std::size_t bytes);
    void removeEstimatedMemory(std::size_t bytes);
    void addSwapBytes(std::size_t bytes);

    void markBookStart(const std::string& instrument);
    void markBookStop(const std::string& instrument);

    double totalMs() const;
    std::size_t currentEstimatedMemory() const;
    std::size_t peakEstimatedMemory() const;
    std::size_t totalSwapBytes() const;
    std::uint64_t ordersRead() const;
    std::uint64_t ordersProcessed() const;
    std::uint64_t reportsGenerated() const;

    void printSummary() const;

private:
    std::chrono::high_resolution_clock::time_point start_{};
    std::chrono::high_resolution_clock::time_point stop_{};

    std::atomic<std::uint64_t> ordersRead_{0};
    std::atomic<std::uint64_t> ordersProcessed_{0};
    std::atomic<std::uint64_t> reportsGenerated_{0};
    std::atomic<std::size_t> currentEstimatedMemory_{0};
    std::atomic<std::size_t> peakEstimatedMemory_{0};
    std::atomic<std::size_t> totalSwapBytes_{0};

    mutable std::mutex bookMutex_;
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> bookStarts_;
    std::unordered_map<std::string, double> bookDurationsMs_;
};

#endif
