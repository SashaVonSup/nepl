/** @file
 * @brief Internal header for IAstNode structure and its derivatives
 */

#ifndef NEPL_AST_H
#define NEPL_AST_H

#include <variant>
#include <memory>
#include <vector>
#include "common.h"

namespace nepl {
    /// Basic structure for a node of AST (abstract syntax tree)
    class IAstNode {
    public:
        virtual ~IAstNode() = default;
    };

    /// Value of literal
    using LiteralValue = std::variant<Integer, Float, std::string>;

    /// AST node for literals
    class LiteralAstNode : public IAstNode {
    protected:
        /// Integer, Float or string
        LiteralValue value;

    public:
        explicit LiteralAstNode(LiteralValue value);
    };

    /// AST node for accessing a variable
    class IdentifierAstNode : public IAstNode {
    protected:
        /// Name of the variable
        std::string name;

    public:
        explicit IdentifierAstNode(std::string name);
    };

    /// AST node for accessing member of an object
    class MemberAstNode : public IAstNode {
    protected:
        /// Name of the member
        std::string name;

        /// Pointer to the parent object
        std::unique_ptr<IAstNode> parent;

    public:
        MemberAstNode(std::string name, std::unique_ptr<IAstNode> parent);
    };

    /// AST node for function call
    class CallAstNode : public IAstNode {
    protected:
        /// Pointer to the function
        std::unique_ptr<IAstNode> function;

        /// List of pointers to arguments
        std::vector<std::unique_ptr<IAstNode>> args;

    public:
        CallAstNode(std::unique_ptr<IAstNode> function, std::vector<std::unique_ptr<IAstNode>> args);
    };

    /// AST node for accessing item by index
    class IndexAstNode : public IAstNode {
    protected:
        /// Pointer to the container object
        std::unique_ptr<IAstNode> container;

        /// Pointer to the index object
        std::unique_ptr<IAstNode> index;

    public:
        IndexAstNode(std::unique_ptr<IAstNode> container, std::unique_ptr<IAstNode> index);
    };
}

#endif //NEPL_AST_H
