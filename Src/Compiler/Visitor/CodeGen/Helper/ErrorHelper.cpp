#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <stdexcept>
#include <string>

void ErrorHelper::errorCompilation(const std::string& message) {
    throw std::runtime_error("Error sémantique : " + message);
}
