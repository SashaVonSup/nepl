#include "Lexer.h"

#include <utility>
#include <regex>
#include <map>

namespace nepl {
    Lexer::Lexer(std::istream &source) : line(0u), curChar(), source(source) {
        nextChar();
    }

    char Lexer::nextChar() {
        return curChar = static_cast<char>(source.get());
    }

    std::vector<Token> Lexer::getTokens() {
        std::vector<Token> res;
        while (!source.eof())
            res.push_back(nextToken());
        return res;
    }

    Token Lexer::tokenize(const std::string &text) const {
        static const std::regex INTEGER(R"(^\d+$)"), FLOAT(R"(^\d*\.\d+$)");
        if (text == "$OPERATOR")
            return {TokenType::OPERATOR, nullptr, line};
        if (text == "$UNOPERATOR")
            return {TokenType::UNOPERATOR, nullptr, line};
        if (std::regex_match(text, INTEGER))
            return {TokenType::INTEGER, Integer(text), line};
        if (std::regex_match(text, FLOAT))
            return {TokenType::FLOAT, Float(text), line};
        return {TokenType::IDENTIFIER, text, line};
    }

    Token Lexer::nextToken() {
        static std::string buffer;
        static bool addDot = false, saveBuffer = false;
        if (addDot) {
            addDot = false;
            return {TokenType::DOT, nullptr, line};
        }
        if (saveBuffer)
            saveBuffer = false;
        else
            buffer.clear();

        while (true) {
            if (curChar == '\n') {
                nextChar();
                return {TokenType::SEMICOLON, nullptr, line++};
            } else if (curChar == '"' || curChar == '\'') {
                auto quote = curChar;
                std::string string;
                while (nextChar() != quote) {
                    if (curChar == '\\') {
                        static std::map<char, char> ESCAPE_SEQUENCES = {
                                {'a', '\a'},
                                {'b', '\b'},
                                {'f', '\f'},
                                {'n', '\n'},
                                {'r', '\r'},
                                {'t', '\t'},
                                {'v', '\v'}
                        };
                        string.push_back(ESCAPE_SEQUENCES.contains(nextChar()) ? ESCAPE_SEQUENCES[curChar] : curChar);
                    } else {
                        string.push_back(curChar);
                        if (curChar == '\n')
                            ++line;
                    }
                }
                nextChar();
                return {TokenType::STRING, string, line};
            } else if (curChar == '#') {
                while (nextChar() != '\n');
                ++line;
                nextChar();
            } else if (curChar == '\\') {
                while (nextChar() != '\n')
                    if (!isspace(nextChar()))
                        throw SyntaxError("unexpected characters after backslash", line);
                ++line;
                nextChar();
            } else {
                static const std::string KEY_CHARACTERS = "()[]{};,.";
                std::string::size_type i;
                if ((i = KEY_CHARACTERS.find(curChar)) != std::string::npos) {
                    nextChar();
                    return {static_cast<TokenType>(static_cast<std::size_t>(TokenType::LEFT_PARENTHESIS) + i),
                            nullptr, line};
                }
            }

            static const std::string BREAK_CHARS = "\"'#\\()[]{};,";
            while (isspace(curChar)) {
                if (curChar == '\n') {
                    nextChar();
                    return {TokenType::SEMICOLON, nullptr, line++};
                }
                nextChar();
            }
            for (bool digits = true, decPoint = false;
                 source && !isspace(curChar) && BREAK_CHARS.find(curChar) == std::string::npos; nextChar()) {
                if (curChar == '.') {
                    if (!digits)
                        return tokenize(buffer);
                    if (decPoint)
                        return tokenize(buffer); //TokenType::FLOAT
                    buffer.push_back(curChar);
                    decPoint = true;
                } else {
                    buffer.push_back(curChar);
                    if (!isdigit(curChar)) {
                        digits = false;
                        if (decPoint) {
                            auto pointPos = buffer.find('.');
                            auto beforePoint = buffer.substr(0, pointPos);
                            buffer = buffer.substr(pointPos + 1);
                            addDot = true;
                            saveBuffer = true;
                            nextChar();
                            return tokenize(beforePoint); //TokenType::INTEGER
                        }
                    }
                }
            }
            if (!buffer.empty())
                return tokenize(buffer);
            else if (!source)
                return {TokenType::SEMICOLON, nullptr, line};
        }
    }
}
