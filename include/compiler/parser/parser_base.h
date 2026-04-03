#ifndef DD335087_6EDE_4036_872C_8BD586E26251
#define DD335087_6EDE_4036_872C_8BD586E26251

#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include <cstddef>
#include <llvm/ADT/ArrayRef.h>
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <vector>
#include <string>

class ParserBase
{
protected:
    // Consumes a token of the expected type, throws if not found
    static auto consume(std::vector<Token>& tokens, std::size_t index, TokenType expectedType, const std::string& errorMessage) -> Token;
    // Consumes the child body until the end token
    static auto consumeChildBody(std::vector<Token>& tokens, std::size_t index, IBuilderTree* builderTree, TokenType end) -> llvm::ArrayRef<INode*>;
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26251 */
