#include "Validator.h"
#include "Utils.h"

ValidationResult Validator::validate(const Order& order) const {
    // Validate Client Order ID
    const std::string& id = order.clientOrderId;
    if (id.empty()) {
        return {false, "Missing Client Order ID"};
    }
    if (id.length() > 7) {
        return {false, "Client Order ID Too Long"};
    }
    for (unsigned char c : id) {
        if (!((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'))) {
            return {false, "Client Order ID Not Alphanumeric"};
        }
    }

    // Validate Instrument
    const std::string& instrument = order.instrument;
    if (instrument.empty()) {
        return {false, "Missing Instrument"};
    }
    if (!Utils::isValidInstrument(instrument)) {
        return {false, "Invalid Instrument"};
    }

    //validate Side
    if (!(order.side == Side::Buy || order.side == Side::Sell)) {
        return {false, "Invalid Side"};
    }

    //validate Price 
    if (order.price <= 0.0) {
        return {false, "Invalid Price"};
    }

    //validate Quantity
    if (order.quantity < 10 ) {
        return {false, "Invalid Quantity Range"};
    }
    if (order.quantity % 10 != 0) {
        return {false, "Quantity Not Multiple Of 10"};
    }
    return {true, ""};
}
