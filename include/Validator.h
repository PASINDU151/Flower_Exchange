#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include "Order.h"

struct ValidationResult {
    bool valid = true;
    std::string reason;
};

class Validator {
public:
    ValidationResult validate(const Order& order) const;
};

#endif
