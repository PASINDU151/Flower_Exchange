#include "Metrics.h"
#include <iomanip>
#include <iostream>

void Metrics::start() {
    start_ = std::chrono::high_resolution_clock::now();
}

void Metrics::stop() {
    stop_ = std::chrono::high_resolution_clock::now();
}

void Metrics::incrementOrdersRead() {
    ++ordersRead_;
}

void Metrics::incrementOrdersProcessed() {
    ++ordersProcessed_;
}

void Metrics::incrementReportsGenerated() {
    ++reportsGenerated_;
}

void Metrics::addEstimatedMemory(std::size_t bytes) {
    std::size_t current = currentEstimatedMemory_.fetch_add(bytes) + bytes;
    std::size_t peak = peakEstimatedMemory_.load();
    while (current > peak && !peakEstimatedMemory_.compare_exchange_weak(peak, current)) {}
}

void Metrics::removeEstimatedMemory(std::size_t bytes) {
    currentEstimatedMemory_.fetch_sub(bytes);
}

void Metrics::addSwapBytes(std::size_t bytes) {
    totalSwapBytes_.fetch_add(bytes);
}

void Metrics::markBookStart(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(bookMutex_);
    bookStarts_[instrument] = std::chrono::high_resolution_clock::now();
}

void Metrics::markBookStop(const std::string& instrument) {
    auto now = std::chrono::high_resolution_clock::now();
    std::lock_guard<std::mutex> lock(bookMutex_);
    auto it = bookStarts_.find(instrument);
    if (it != bookStarts_.end()) {
        double ms = std::chrono::duration<double, std::milli>(now - it->second).count();
        bookDurationsMs_[instrument] += ms;
        bookStarts_.erase(it);
    }
}

double Metrics::totalMs() const {
    return std::chrono::duration<double, std::milli>(stop_ - start_).count();
}

std::size_t Metrics::currentEstimatedMemory() const {
    return currentEstimatedMemory_.load();
}

std::size_t Metrics::peakEstimatedMemory() const {
    return peakEstimatedMemory_.load();
}

std::size_t Metrics::totalSwapBytes() const {
    return totalSwapBytes_.load();
}

std::uint64_t Metrics::ordersRead() const {
    return ordersRead_.load();
}

std::uint64_t Metrics::ordersProcessed() const {
    return ordersProcessed_.load();
}

std::uint64_t Metrics::reportsGenerated() const {
    return reportsGenerated_.load();
}

void Metrics::printSummary() const {
    std::cout << "\n===== PERFORMANCE SUMMARY =====\n";
    std::cout << "Orders read            : " << ordersRead() << "\n";
    std::cout << "Orders processed       : " << ordersProcessed() << "\n";
    std::cout << "Reports generated      : " << reportsGenerated() << "\n";
    std::cout << "Total runtime (ms)     : " << std::fixed << std::setprecision(3) << totalMs() << "\n";
    std::cout << "Estimated current RAM  : " << currentEstimatedMemory() << " bytes\n";
    std::cout << "Estimated peak RAM     : " << peakEstimatedMemory() << " bytes\n";
    std::cout << "Secondary storage used : " << totalSwapBytes() << " bytes\n";

    if (!bookDurationsMs_.empty()) {
        std::cout << "\nPer-book active durations:\n";
        for (const auto& [book, ms] : bookDurationsMs_) {
            std::cout << "  " << book << " : " << ms << " ms\n";
        }
    }
    std::cout << "===============================\n";
}
