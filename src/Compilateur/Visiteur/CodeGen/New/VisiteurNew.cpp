#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include <stdexcept>

void VisiteurGeneralGenCode::visiter(NoeudNew* noeudNew)
{
    auto& module = _contextGenCode->backend->getModule();
    auto& builder = _contextGenCode->backend->getBuilder();

    llvm::Type* typeCible = nullptr;
    Class* infoClasse = nullptr;

    if (noeudNew->getNomType().type == TOKEN_IDENTIFIANT) {
        infoClasse = _contextGenCode->registreClass->recuperer(noeudNew->getNomType().value);
        if (infoClasse != nullptr) {
            typeCible = infoClasse->structType;
        }
    } else {
        typeCible = _contextGenCode->registreType->recuperer(noeudNew->getNomType().type);
    }

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

    // Il faut remplir le vecteur d'argument du constructeur avec les argument du noeudNew exemple ; new MaClasse(arg1, arg2)
    std::vector<llvm::Value*> argsConstructeur;
    argsConstructeur.push_back(adresseAllouee);  // this

    // Ajouter les arguments passés au new (les enfants du noeud)
    for (INoeud* arg : noeudNew->getArguments()) {
        arg->accept(this);  // Évalue l'expression (ex: entier = 204)
        argsConstructeur.push_back(_contextGenCode->valeurTemporaire.adresse);
    }

    if (infoClasse != nullptr && infoClasse->vtable != nullptr) {
        // Initialiser le vptr à l'adresse 0 de l'objet alloué
        llvm::Value* vptrAdresse = builder.CreateStructGEP(typeCible, adresseAllouee, 0, "vptr_adresse");
        llvm::Value* vtablePtr = builder.CreateBitCast(infoClasse->vtable, builder.getPtrTy());
        builder.CreateStore(vtablePtr, vptrAdresse);
    }

    if (infoClasse != nullptr) {
        for (const auto& pair : infoClasse->memberInitializers) {
            const std::string& nomMembre = pair.first;
            INoeud* expressionInit = pair.second;

            if (expressionInit != nullptr) {
                expressionInit->accept(this);
                llvm::Value* valeurCalculee = _contextGenCode->valeurTemporaire.adresse;

                if (valeurCalculee != nullptr && infoClasse->memberIndices.find(nomMembre) != infoClasse->memberIndices.end()) {
                    unsigned int idx = infoClasse->memberIndices[nomMembre];
                    
                    Token tokenMembre; tokenMembre.value = nomMembre;
                    Symbole modele = infoClasse->registreVariable->recupererVariables(tokenMembre);
                    llvm::Type* typeMembre = modele.type->genererTypeLLVM(_contextGenCode->backend->getContext());

                    llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurCalculee, typeMembre);
                    llvm::Value* membrePtr = builder.CreateStructGEP(typeCible, adresseAllouee, idx, nomMembre + "_ptrinit");
                    builder.CreateStore(valeurCastee, membrePtr);
                }
            }
        }
    }

    // Construction du constructeur avec arguments
    if (infoClasse != nullptr) {
        std::string nomConstructeur = noeudNew->getNomType().value;
        if (infoClasse->registreFonctionLocale->existe(nomConstructeur)) {
            const auto& symbolePtr = infoClasse->registreFonctionLocale->recuperer(nomConstructeur);
            const auto* symboleFonction = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
            builder.CreateCall(symboleFonction->fonction, argsConstructeur);
        }
    }

    _contextGenCode->valeurTemporaire.adresse = adresseAllouee;
    _contextGenCode->valeurTemporaire.typePointeElement = typeCible;
}