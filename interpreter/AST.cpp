#include "AST.h"

#include <utility>

namespace nepl {
    LiteralAstNode::LiteralAstNode(LiteralValue value) : value(std::move(value)) {}

    IdentifierAstNode::IdentifierAstNode(std::string name) : name(std::move(name)) {}

    MemberAstNode::MemberAstNode(std::string name, std::unique_ptr<IAstNode> parent) :
            name(std::move(name)), parent(std::move(parent)) {}

    CallAstNode::CallAstNode(std::unique_ptr<IAstNode> function, std::vector<std::unique_ptr<IAstNode>> args) :
            function(std::move(function)), args(std::move(args)) {}

    IndexAstNode::IndexAstNode(std::unique_ptr<IAstNode> container, std::unique_ptr<IAstNode> index) :
            container(std::move(container)), index(std::move(index)) {}
}
