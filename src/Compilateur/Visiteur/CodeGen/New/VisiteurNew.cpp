#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/New/NoeudNew.h"
#include <stdexcept>

void VisiteurGeneralGenCode::visiter(NoeudNew* noeudNew)
{
    auto& module = _contextGenCode->backend->getModule();
    auto& builder = _contextGenCode->backend->getBuilder();

    // On va le chercher dans ton registre via le token stocké dans le noeud
    llvm::Type* typeCible = _contextGenCode->registreType->recuperer(noeudNew->getNomType().type);

    if (typeCible == nullptr) {
        throw std::runtime_error("Erreur: le type cible n'existe pas! ");
    }

    // C'est ici que LLVM décide si un int32 fait 4 octets, etc.
    const llvm::DataLayout& dataLayout = module.getDataLayout();
    uint64_t tailleOctets = dataLayout.getTypeAllocSize(typeCible);
    
    // On transforme ce chiffre en valeur LLVM (i64) pour l'argument de malloc
    llvm::Value* tailleLLVM = builder.getInt64(tailleOctets);

    llvm::Function* mallocFunc = module.getFunction("prysma_malloc");

    if (mallocFunc == nullptr) {
        throw std::runtime_error("Erreur: la fonction prysma_malloc n'existe pas! ");
    }

    llvm::Value* adresseAllouee = builder.CreateCall(mallocFunc, {tailleLLVM}, "memoire_new");

    _contextGenCode->valeurTemporaire.adresse = adresseAllouee;
}