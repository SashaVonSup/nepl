#include "Token.h"

#include <utility>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const TokenValue &value) {
    switch (value.index()) {
        case 0:
            return os << get<0>(value);
        case 1:
            return os << get<1>(value);
        case 2:
            return os << get<2>(value);
        default:
            return os; //unexpected
    }
}

namespace nepl {
    std::ostream &operator<<(std::ostream &os, const TokenType &type) {
        static const char *TOKEN_TYPES[] = {
                "IDENTIFIER", "STRING", "INTEGER", "FLOAT", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS",
                "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", "LEFT_BRACE", "RIGHT_BRACE",
                "SEMICOLON", "COMMA", "DOT", "OPERATOR", "UNOPERATOR",
        };
        return os << TOKEN_TYPES[static_cast<unsigned>(type)];
    }

    TokenType operator+(TokenType type, char add) {
        return TokenType(static_cast<unsigned char>(type) + add);
    }

    Token::Token(TokenType type, TokenValue value, unsigned line) : type(type), value(std::move(value)), line(line) {}

    std::ostream &operator<<(std::ostream &os, const Token &token) {
        os << token.type << '@' << token.line;
        return token.value.index() == 3 ? os : os << ": " << token.value;
    }
}
