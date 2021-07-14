/** @file
 * @brief Header for common types, etc.
 */

#ifndef NEPL_COMMON_H
#define NEPL_COMMON_H

#include <gmpxx.h>
#include <exception>

namespace nepl {
    /// Big integer number
    typedef mpz_class Integer;

    /// Big floating point number
    typedef mpf_class Float;

    /// Error because of invalid source code or programmer's mistake
    class SyntaxError : public std::exception {
    protected:
        /// Error description
        const std::string message;

        /// Number of the line where the mistake is
        const unsigned line;

    public:
        SyntaxError(std::string message, unsigned line);

        [[nodiscard]] const char *what() const noexcept override;
    };
}

#endif //NEPL_COMMON_H