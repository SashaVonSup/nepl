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

    std::vector<std::unique_ptr<IAstNode>> Parser::getAstNodes() {
        std::vector<std::unique_ptr<IAstNode>> res;
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

    std::unique_ptr<CallAstNode> Parser::parseCall(std::unique_ptr<IAstNode> function) {
        std::vector<std::unique_ptr<IAstNode>> args;
        while (nextToken().type != TokenType::RIGHT_PARENTHESIS) {
            args.push_back(parse());
            if (nextToken().type != TokenType::COMMA)
                throw SyntaxError("comma", curToken.type, curToken.line);
        }
        return std::make_unique<CallAstNode>(std::move(function), std::move(args));
    }

    std::unique_ptr<IAstNode> Parser::parse() {
        std::unique_ptr<IAstNode> node;
        switch (curToken.type) {
            case TokenType::IDENTIFIER:
                node = std::make_unique<IdentifierAstNode>(get<0>(curToken.value));
                break;
            case TokenType::STRING:
                node = std::make_unique<LiteralAstNode>(get<0>(curToken.value));
                break;
            case TokenType::INTEGER:
                node = std::make_unique<LiteralAstNode>(get<1>(curToken.value));
                break;
            case TokenType::FLOAT:
                node = std::make_unique<LiteralAstNode>(get<2>(curToken.value));
                break;
            default:
                throw SyntaxError(curToken.type, curToken.line);
        }

        while (true) {
            switch (nextToken().type) {
                case TokenType::DOT:
                    if (nextToken().type != TokenType::IDENTIFIER)
                        throw SyntaxError("member identifier", curToken.type, curToken.line);
                    node = std::make_unique<MemberAstNode>(get<0>(curToken.value), std::move(node));
                    break;
                case TokenType::LEFT_PARENTHESIS:
                    nextToken();
                    node = parseCall(std::move(node));
                    break;
                case TokenType::LEFT_SQUARE_BRACKET:
                    nextToken();
                    node = std::make_unique<IndexAstNode>(std::move(node), parse());
                    break;
                default:
                    return node;
            }
        }
    }

    std::unique_ptr<IAstNode> Parser::nextAstNode() {
        while (true) {
            switch (curToken.type) {
                case TokenType::OPERATOR:
                    declareOperator();
                    break;
                case TokenType::UNOPERATOR:
                    disableOperator();
                    break;
                case TokenType::SEMICOLON:
                    nextToken();
                    break;
                default:
                    auto node = parse();
                    if (curToken.type != TokenType::SEMICOLON)
                        throw SyntaxError(curToken.type, curToken.line);
                    nextToken(); //TokenType::SEMICOLON
                    return node;
            }
        }
    }
}
