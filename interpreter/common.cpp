#include "common.h"

#include <utility>

namespace nepl {
    SyntaxError::SyntaxError(std::string message, unsigned line) : message(std::move(message)), line(line) {}

    const char *SyntaxError::what() const noexcept {
        static const auto text = message + " in line " + std::to_string(line);
        return text.c_str();
    }
}
