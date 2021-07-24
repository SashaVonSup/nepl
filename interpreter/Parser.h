/** @file
 * @brief Header for Parser class
 */

#ifndef NEPL_PARSER_H
#define NEPL_PARSER_H

#include <map>
#include "Lexer.h"
#include "AST.h"

namespace nepl {
    /// Symbol of operator, i.e. its appearance
    struct OperatorSymbol {
        /// List of operator's elements (1 for unary and binary, 2 for ternary, etc.)
        std::vector<std::string> elements;

        /// Is the operator unary?
        bool isUnary;

        explicit OperatorSymbol(bool isUnary = false);

        explicit OperatorSymbol(std::vector<std::string> elements, bool isUnary = false);

        /// Number of the elements, same as elements.size
        [[nodiscard]] inline size_t size() const noexcept;

        /// Add an element at the end, same as elements.push_back
        inline void push_back(const std::string &element);

        /// Comparison, needed for sorting
        inline bool operator<(const OperatorSymbol &other) const;
    };

    /// Value of operator declaration
    struct OperatorValue {
        /// Identifier (name) of the linked function
        std::string function;

        /// Operator's precedence (less number means higher precedence)
        Integer precedence;

        OperatorValue(std::string function, Integer precedence);

        explicit OperatorValue(std::string function);
    };

    /// Syntax analyzer, builds abstract syntax tree (AST)
    class Parser {
    protected:
        /// Factory of tokens to analyze
        Lexer lexer;

        /// Current Token
        Token curToken;

        /// Get next character from lexer
        Token nextToken();

        /// List of operators declared in the program
        std::map<OperatorSymbol, OperatorValue> operators;

        /// Parse operator declaration and add it to operators
        void declareOperator();

        /// Parse operator disabling and remove from operators
        void disableOperator();

        /// Parse arguments for function call
        std::unique_ptr<CallAstNode> parseCall(std::unique_ptr<IAstNode> function);

        /// Make an AST node from tokens
        std::unique_ptr<IAstNode> parse();

    public:
        explicit Parser(Lexer lexer);

        /// Get next built tree
        std::unique_ptr<IAstNode> nextAstNode();

        /// Result list of built trees
        std::vector<std::unique_ptr<IAstNode>> getAstNodes();
    };
}

#endif //NEPL_PARSER_H
