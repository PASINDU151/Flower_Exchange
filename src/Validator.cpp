#include "Validator.h"
#include "Utils.h"

ValidationResult Validator::validate(const Order& order) const {
    if (order.clientOrderId.empty()) {
        return {false, "Missing Client Order ID"};
    }
    if (order.instrument.empty()) {
        return {false, "Missing Instrument"};
    }
    if (!Utils::isValidInstrument(order.instrument)) {
        return {false, "Invalid Instrument"};
    }
    if (!(order.side == Side::Buy || order.side == Side::Sell)) {
        return {false, "Invalid Side"};
    }
    if (order.price <= 0.0) {
        return {false, "Invalid Price"};
    }
    if (order.quantity < 10 || order.quantity > 1000) {
        return {false, "Invalid Quantity Range"};
    }
    if (order.quantity % 10 != 0) {
        return {false, "Quantity Not Multiple Of 10"};
    }
    return {true, ""};
}
