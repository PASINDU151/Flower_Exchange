#include "Utils.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
    std::atomic<unsigned long long> g_orderCounter{1};
}

std::string Utils::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::vector<std::string> Utils::splitCsvLine(const std::string& line) {
    std::vector<std::string> out;
    std::string cur;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            out.push_back(trim(cur));
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    out.push_back(trim(cur));
    return out;
}

std::string Utils::currentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto timeT = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tmBuf{};
#ifdef _WIN32
    localtime_s(&tmBuf, &timeT);
#else
    localtime_r(&timeT, &tmBuf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y%m%d-%H%M%S")
        << "." << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}

std::string Utils::generateSystemOrderId() {
    unsigned long long id = g_orderCounter.fetch_add(1);
    return "ord" + std::to_string(id);
}

Side Utils::intToSide(int side) {
    if (side == 1) return Side::Buy;
    if (side == 2) return Side::Sell;
    return Side::Unknown;
}

int Utils::sideToInt(Side side) {
    if (side == Side::Buy) return 1;
    if (side == Side::Sell) return 2;
    return 0;
}

std::string Utils::sideToString(Side side) {
    if (side == Side::Buy) return "Buy";
    if (side == Side::Sell) return "Sell";
    return "Unknown";
}

std::string Utils::execStatusToString(ExecStatus status) {
    switch (status) {
        case ExecStatus::New: return "New";
        case ExecStatus::Rejected: return "Rejected";
        case ExecStatus::Fill: return "Fill";
        case ExecStatus::PFill: return "PFill";
        default: return "Unknown";
    }
}

bool Utils::isValidInstrument(const std::string& instrument) {
    static const std::vector<std::string> valid = {
        "Rose", "Lavender", "Lotus", "Tulip", "Orchid"
    };
    return std::find(valid.begin(), valid.end(), instrument) != valid.end();
}
