/** @file
 * @brief Header for common types, etc.
 */

#ifndef NEPL_COMMON_H
#define NEPL_COMMON_H

#include <gmpxx.h>
#include <string>
#include <exception>
#include <sstream>

namespace nepl {
    /// Big integer number
    typedef mpz_class Integer;

    /// Big floating point number
    typedef mpf_class Float;

    /// Error because of invalid source code or programmer's mistake
    class SyntaxError : public std::exception {
    protected:
        /// Error description
        std::string message;

        /// Number of the line where the mistake is
        const unsigned line;

    public:
        SyntaxError(std::string message, unsigned line);

        template<typename T>
        SyntaxError(const T &unexpected, unsigned line) :
                message((std::stringstream() << "unexpected " << unexpected).str()), line(line) {}

        template<typename T1, typename T2>
        SyntaxError(const T1 &expected, const T2 &found, unsigned line) :
                message((std::stringstream() << "expected " << expected << ", found " << found).str()), line(line) {}

        [[nodiscard]] const char *what() const noexcept override;
    };
}

#endif //NEPL_COMMON_H
