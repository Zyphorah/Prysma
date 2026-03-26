#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <stdexcept>
#include <string>

void ErrorHelper::compilationError(const std::string& message) {
    throw std::runtime_error("Semantic error: " + message);
}
