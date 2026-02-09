#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include <memory>
#include "Compilateur/Lexer/TokenType.h"

class RegistreInstruction : public RegistreGeneric<TokenType, std::shared_ptr<IParseur>> {
public:
    RegistreInstruction() = default;
    virtual ~RegistreInstruction() = default;

};
