/** @file
 * @brief Internal header for Token structure, q.v. Lexer.h
 */

#ifndef NEPL_TOKEN_H
#define NEPL_TOKEN_H

#include <variant>
#include <string>
#include "common.h"

namespace nepl {
    /// Type of token
    enum class TokenType : unsigned char {
        IDENTIFIER = 0, ///< May be name of variable; requires std::string value
        STRING, ///< String literal; requires std::string value
        INTEGER, ///< Integer literal; requires Integer value
        FLOAT, ///< Floating point literal; requires Float value
        LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET, LEFT_BRACE, RIGHT_BRACE,
        SEMICOLON, ///< Semicolon (;) or end of line without backslash (\)
        COMMA, DOT,
        OPERATOR, UNOPERATOR,
    };

    std::ostream &operator<<(std::ostream &os, const TokenType &type);

    /// Value of Token dictated by TokenType
    using TokenValue = std::variant<std::string, Integer, Float, nullptr_t>;

    std::ostream &operator<<(std::ostream &os, const TokenValue &value);

    /// Minimal significant piece of code (a word or operator, etc.)
    struct Token {
        /// What the token represents
        TokenType type;

        /// Value typed by TokenType
        TokenValue value;

        /// Number of the line of source code file where the token is
        unsigned line;

        Token(TokenType type, TokenValue value, unsigned line);
    };

    std::ostream &operator<<(std::ostream &os, const Token &token);
}

#endif //NEPL_TOKEN_H
