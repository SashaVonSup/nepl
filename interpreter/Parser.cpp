#include "Parser.h"

#include <stack>

namespace nepl {
    OperatorSymbol::OperatorSymbol(bool isUnary) : isUnary(isUnary) {}

    OperatorSymbol::OperatorSymbol(std::vector<std::string> elements, bool isUnary) :
            elements(std::move(elements)), isUnary(isUnary) {}

    inline size_t OperatorSymbol::size() const noexcept {
        return elements.size();
    }

    inline void OperatorSymbol::push_back(const std::string &element) {
        return elements.push_back(element);
    }

    bool OperatorSymbol::operator<(const OperatorSymbol &other) const {
        return isUnary == other.isUnary ? elements < other.elements : isUnary < other.isUnary;
    }

    OperatorValue::OperatorValue(std::string function, Integer precedence) :
            function(std::move(function)), precedence(std::move(precedence)) {}

    OperatorValue::OperatorValue(std::string function) : function(std::move(function)) {}

    Parser::Parser(Lexer lexer) : lexer(lexer), curToken(nextToken()), operators() {}

    Token Parser::nextToken() {
        return curToken = lexer.nextToken();
    }

    std::vector<AstNode> Parser::getAstNodes() {
        std::vector<AstNode> res;
        while (!lexer.source.eof())
            res.push_back(nextAstNode());
        return res;
    }

    void Parser::declareOperator() {
        if (nextToken().type != TokenType::IDENTIFIER)
            throw SyntaxError("function identifier for operator declaration", curToken.type, curToken.line);
        OperatorValue value(get<0>(curToken.value));
        OperatorSymbol symbol;
        switch (nextToken().type) {
            case TokenType::INTEGER:
                value.precedence = std::move(get<1>(curToken.value));
                break;
            case TokenType::IDENTIFIER:
                if (get<0>(curToken.value) != "$UNARY")
                    throw SyntaxError("integer precedence or $UNARY directive", curToken.value, curToken.line);
                symbol.isUnary = true;
                if (nextToken().type != TokenType::INTEGER)
                    throw SyntaxError("integer precedence", curToken.type, curToken.line);
                value.precedence = std::move(get<1>(curToken.value));
                break;
            default:
                throw SyntaxError("integer precedence or $UNARY directive", curToken.value, curToken.line);
        }
        nextToken(); //TokenType::INTEGER

        do {
            if (curToken.type != TokenType::IDENTIFIER)
                throw SyntaxError("element of operator symbol", curToken.value, curToken.line);
            symbol.push_back(get<0>(curToken.value));
        } while (nextToken().type != TokenType::SEMICOLON);
        if (symbol.isUnary && symbol.size() != 1)
            throw SyntaxError(
                    "declaring unary operator with " + std::to_string(symbol.size()) + " elements",
                    curToken.line);

        if (operators.contains(symbol))
            throw SyntaxError("redeclaring operator", curToken.line);
        operators.insert({symbol, value});
        nextToken(); //TokenType::SEMICOLON
    }

    void Parser::disableOperator() {
        OperatorSymbol symbol;
        if (nextToken().type != TokenType::IDENTIFIER)
            throw SyntaxError("element of operator symbol or $UNARY directive", curToken.type, curToken.line);
        if (get<0>(curToken.value) == "$UNARY")
            symbol.isUnary = true;
        else
            symbol.push_back(get<0>(curToken.value));

        while (nextToken().type != TokenType::SEMICOLON) {
            if (curToken.type != TokenType::IDENTIFIER)
                throw SyntaxError("element of operator symbol", curToken.value, curToken.line);
            symbol.push_back(get<0>(curToken.value));
        }

        if (!operators.contains(symbol))
            throw SyntaxError("disabling undeclared operator", curToken.line);
        operators.erase(symbol);
        nextToken(); //TokenType::SEMICOLON
    }

    //TODO: AstNode Parser::parse(std::vector<Token> tokens)

    AstNode Parser::nextAstNode() {
        while (true) {
            if (curToken.type == TokenType::OPERATOR) {
                declareOperator();
            } else if (curToken.type == TokenType::UNOPERATOR) {
                disableOperator();
            }

            std::vector<Token> tokens;
            for (std::stack<Token> brackets; curToken.type != TokenType::SEMICOLON || !brackets.empty(); nextToken()) {
                switch (curToken.type) {
                    case TokenType::LEFT_PARENTHESIS:
                    case TokenType::LEFT_SQUARE_BRACKET:
                    case TokenType::LEFT_BRACE:
                        brackets.push(curToken);
                        break;
                    case TokenType::RIGHT_PARENTHESIS:
                    case TokenType::RIGHT_SQUARE_BRACKET:
                    case TokenType::RIGHT_BRACE:
                        if (brackets.empty())
                            throw SyntaxError((std::stringstream() << "unexpected " << curToken.type).str(),
                                              curToken.line);
                        if (curToken.type == brackets.top().type + 1)
                            brackets.pop();
                        else
                            throw SyntaxError(brackets.top().type + 1, curToken.type, curToken.line);
                    default:
                        break;
                }
                tokens.push_back(curToken);
            }

            if (tokens.empty())
                continue;
            return parse(tokens);
        }
    }
}
