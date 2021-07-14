#include "AST.h"

#include <utility>

namespace nepl {
    LiteralAstNode::LiteralAstNode(LiteralValue value) : value(std::move(value)) {}

    NameAstNode::NameAstNode(std::string name) : name(std::move(name)) {}

    MemberAstNode::MemberAstNode(std::string name, std::unique_ptr<AstNode> parent) :
            name(std::move(name)), parent(std::move(parent)) {}

    CallAstNode::CallAstNode(std::vector<std::unique_ptr<AstNode>> args) : args(std::move(args)) {}

    IndexAstNode::IndexAstNode(std::unique_ptr<AstNode> container, std::unique_ptr<AstNode> index) :
            container(std::move(container)), index(std::move(index)) {}
}
