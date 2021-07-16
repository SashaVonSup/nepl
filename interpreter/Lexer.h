/** @file
 * @brief Header for Lexer class
 */

#ifndef NEPL_LEXER_H
#define NEPL_LEXER_H

#include <iostream>
#include <vector>
#include "Token.h"

namespace nepl {
    /// Lexical analyzer, transforms source code to Token list
    class Lexer {
    protected:
        /// Number of current line
        unsigned line;

        /// Current character
        char curChar;

        /// Get next character from source
        char nextChar();

        /// Make a token from word
        [[nodiscard]] Token tokenize(const std::string &text) const;

    public:
        /// Input stream with source code to analyze
        std::istream &source;

        explicit Lexer(std::istream &source);

        /// Next found token
        Token nextToken();

        /// Result list of tokens
        std::vector<Token> getTokens();
    };
}

#endif //NEPL_LEXER_H
