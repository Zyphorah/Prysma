#include "Compilateur/AST/Noeuds/NoeudMain.h"

NoeudMain::NoeudMain()
{
}

NoeudMain::~NoeudMain()
{
}

llvm::Value* NoeudMain::genCode()
{
    llvm::Value* resultat = nullptr;
    for (const auto& enfant : enfants) {
        resultat = enfant->genCode();
    }
    return resultat;
}
