#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include <cstdint>
#include <llvm-18/llvm/IR/Value.h>
#include <stdexcept>
#include <string>
#include <vector>

void VisiteurGeneralGenCode::visiter(NoeudNew* noeudNew)
{
    auto& module = _contextGenCode->getBackend()->getModule();
    auto& builder = _contextGenCode->getBackend()->getBuilder();

    llvm::Type* typeCible = nullptr;
    Class* infoClasse = nullptr;

    if (noeudNew->getNomType().type == TOKEN_IDENTIFIANT) {
        infoClasse = _contextGenCode->getRegistreClass()->recuperer(noeudNew->getNomType().value).get();
        infoClasse = ErrorHelper::verifierNonNull(infoClasse, "Classe '" + noeudNew->getNomType().value + "' non trouvée");
        typeCible = infoClasse->getStructType();
    } else {
        typeCible = _contextGenCode->getRegistreType()->recuperer(noeudNew->getNomType().type);
    }

    typeCible = ErrorHelper::verifierNonNull(typeCible, "Type cible non déterminé pour 'new'");

    // C'est ici que LLVM décide si un int32 fait 4 octets, etc.
    const llvm::DataLayout& dataLayout = module.getDataLayout();
    uint64_t tailleOctets = dataLayout.getTypeAllocSize(typeCible);
    
    // On transforme ce chiffre en valeur LLVM (i64) pour l'argument de malloc
    llvm::Value* tailleLLVM = builder.getInt64(tailleOctets);

    llvm::Function* mallocFunc = module.getFunction("prysma_malloc");

    mallocFunc = ErrorHelper::verifierNonNull(mallocFunc, "Fonction prysma_malloc non déclarée dans le module");

    llvm::Value* adresseAllouee = builder.CreateCall(mallocFunc, {tailleLLVM}, "memoire_new");

    // Il faut remplir le vecteur d'argument du constructeur avec les argument du noeudNew exemple ; new MaClasse(arg1, arg2)
    std::vector<llvm::Value*> argsConstructeur;
    argsConstructeur.push_back(adresseAllouee);  // this

    // Ajouter les arguments passés au new (les enfants du noeud)
    for (INoeud* arg : noeudNew->getArguments()) {
        arg->accept(this);  // Évalue l'expression (ex: entier = 204)
        argsConstructeur.push_back(_contextGenCode->getValeurTemporaire().getAdresse());
    }
    
    if (infoClasse != nullptr && infoClasse->getVTable() != nullptr) {
        // Initialiser le vptr à l'adresse 0 de l'objet alloué
        llvm::Value* vptrAdresse = builder.CreateStructGEP(typeCible, adresseAllouee, 0, "vptr_adresse");
        llvm::Value* vtablePtr = builder.CreateBitCast(infoClasse->getVTable(), builder.getPtrTy());
        builder.CreateStore(vtablePtr, vptrAdresse);
    }

    if (infoClasse != nullptr) {
        for (const auto& pair : infoClasse->getMemberInitializers()) {
            const std::string& nomMembre = pair.first;
            INoeud* expressionInit = pair.second;

            if (expressionInit != nullptr) {
                expressionInit->accept(this);
                llvm::Value* valeurCalculee = _contextGenCode->getValeurTemporaire().getAdresse();

                if (valeurCalculee != nullptr && infoClasse->getMemberIndices().find(nomMembre) != infoClasse->getMemberIndices().end()) {
                    unsigned int idx = infoClasse->getMemberIndices()[nomMembre];
                    
                    Token tokenMembre; tokenMembre.value = nomMembre;
                    Symbole modele = infoClasse->getRegistreVariable()->recupererVariables(tokenMembre);
                    llvm::Type* typeMembre = modele.getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());

                    llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(valeurCalculee, typeMembre);
                    llvm::Value* membrePtr = builder.CreateStructGEP(typeCible, adresseAllouee, idx, nomMembre + "_ptrinit");
                    builder.CreateStore(valeurCastee, membrePtr);
                }
            }
        }
    }

    // Construction du constructeur avec arguments
    if (infoClasse != nullptr) {
        std::string nomConstructeur = noeudNew->getNomType().value;
        if (infoClasse->getRegistreFonctionLocale()->existe(nomConstructeur)) {
            const auto& symbolePtr = infoClasse->getRegistreFonctionLocale()->recuperer(nomConstructeur);
            if (!prysma::isa<SymboleFonctionLocale>(symbolePtr.get())) {
                throw std::runtime_error("Erreur : SymboleFonctionLocale attendu");
            }
            const auto* symboleFonction = prysma::cast<const SymboleFonctionLocale>(symbolePtr.get());
            builder.CreateCall(symboleFonction->fonction, argsConstructeur);
        }
    }

    _contextGenCode->modifierValeurTemporaire(Symbole(adresseAllouee, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), _contextGenCode->getValeurTemporaire().getType(), typeCible));
}