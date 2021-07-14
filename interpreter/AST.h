/** @file
 * @brief Internal header for AstNode structure and its derivatives
 */

#ifndef NEPL_AST_H
#define NEPL_AST_H

#include <variant>
#include <memory>
#include "common.h"

namespace nepl {
    /// Basic structure for a node of AST (abstract syntax tree)
    struct AstNode {};

    /// Value of literal
    using LiteralValue = std::variant<Integer, Float, std::string>;

    /// AST node for literals
    struct LiteralAstNode : AstNode {
        /// Integer, Float or string
        LiteralValue value;

        explicit LiteralAstNode(LiteralValue value);
    };

    /// AST node for accessing a variable
    struct NameAstNode : AstNode {
        /// Name of the variable
        std::string name;

        explicit NameAstNode(std::string name);
    };

    /// AST node for accessing member of an object
    struct MemberAstNode : AstNode {
        /// Name of the member
        std::string name;

        /// Pointer to the parent object
        std::unique_ptr<AstNode> parent;

        MemberAstNode(std::string name, std::unique_ptr<AstNode> parent);
    };

    /// AST node for function call
    struct CallAstNode : AstNode {
        /// List of pointers to arguments
        std::vector<std::unique_ptr<AstNode>> args;

        explicit CallAstNode(std::vector<std::unique_ptr<AstNode>> args);
    };

    /// AST node for accessing item by index
    struct IndexAstNode : AstNode {
        /// Pointer to the container object
        std::unique_ptr<AstNode> container;

        /// Pointer to the index object
        std::unique_ptr<AstNode> index;

        IndexAstNode(std::unique_ptr<AstNode> container, std::unique_ptr<AstNode> index);
    };
}

#endif //NEPL_AST_H
