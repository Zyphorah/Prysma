#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/Lexer/TokenType.h"

class RegistreInstruction : public RegistreGeneric<TokenType, IParseur*> {
public:
    RegistreInstruction() = default;
    virtual ~RegistreInstruction() = default;

};
