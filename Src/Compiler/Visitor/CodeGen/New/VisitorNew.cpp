#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <cstddef>
#include <cstdint>
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

void GeneralVisitorGenCode::visiter(NodeNew* nodeNew)
{
    auto& module = _contextGenCode->getBackend()->getModule();
    auto& builder = _contextGenCode->getBackend()->getBuilder();

    llvm::Type* typeCible = nullptr;
    Class* infoClasse = nullptr;

    if (nodeNew->getNomType().type == TOKEN_IDENTIFIANT) {
        infoClasse = _contextGenCode->getRegistryClass()->recuperer(nodeNew->getNomType().value).get();
        infoClasse = ErrorHelper::verifierNonNull(infoClasse, "Classe '" + nodeNew->getNomType().value + "' non trouvée");
        typeCible = infoClasse->getStructType();
    } else {
        typeCible = _contextGenCode->getRegistryType()->recuperer(nodeNew->getNomType().type);
    }

    typeCible = ErrorHelper::verifierNonNull(typeCible, "Type cible non déterminé pour 'new'");

    // C'est ici que LLVM décide si un int32 fait 4 octets, etc.
    const llvm::DataLayout& dataLayout = module.getDataLayout();
    uint64_t tailleOctets = dataLayout.getTypeAllocSize(typeCible);
    
    // On transforme ce chiffre en valeur LLVM (i64) pour l'argument de malloc
    llvm::Value* tailleLLVM = builder.getInt64(tailleOctets);

    llvm::Function* mallocFunc = module.getFunction("prysma_malloc");

    mallocFunc = ErrorHelper::verifierNonNull(mallocFunc, "Function prysma_malloc non déclarée dans le module");

    llvm::Value* adresseAllouee = builder.CreateCall(mallocFunc, {tailleLLVM}, "memoire_new");

    // Il faut remplir le vecteur d'argument du builder avec les argument du nodeNew exemple ; new MaClasse(arg1, arg2)
    std::vector<llvm::Value*> argsBuilder;
    argsBuilder.push_back(adresseAllouee);  // this

    // Ajouter les arguments passés au new (les childs du node)
    for (INode* arg : nodeNew->getArguments()) {
        arg->accept(this);  // Évalue l'expression (ex: entier = 204)
        argsBuilder.push_back(_contextGenCode->getValeurTemporaire().getAdresse());
    }
    
    if (infoClasse != nullptr && infoClasse->getVTable() != nullptr) {
        // Initialiser le vptr à l'adresse 0 de l'object alloué
        llvm::Value* vptrAdresse = builder.CreateStructGEP(typeCible, adresseAllouee, 0, "vptr_adresse");
        llvm::Value* vtablePtr = builder.CreateBitCast(infoClasse->getVTable(), builder.getPtrTy());
        builder.CreateStore(vtablePtr, vptrAdresse);
    }

    if (infoClasse != nullptr) {
        for (const auto& pair : infoClasse->getMemberInitializers()) {
            const std::string& nomMembre = pair.first;
            INode* expressionInit = pair.second;

            if (expressionInit != nullptr) {
                auto* arrayInit = prysma::dyn_cast<NodeArrayInitialization>(expressionInit);
                
                if (arrayInit != nullptr) {
                    if (infoClasse->getMemberIndices().find(nomMembre) != infoClasse->getMemberIndices().end()) {
                        unsigned int idx = infoClasse->getMemberIndices()[nomMembre];
                        
                        Token tokenMembre; tokenMembre.value = nomMembre;
                        Symbole modele = infoClasse->getRegistryVariable()->recupererVariables(tokenMembre);
                        llvm::Type* typeMembre = modele.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
                        auto* typeArrayLLVM = llvm::dyn_cast<llvm::ArrayType>(typeMembre);
                        if (typeArrayLLVM != nullptr) {
                            llvm::Type* typeElement = typeArrayLLVM->getElementType();
                            llvm::Value* membrePtr = builder.CreateStructGEP(typeCible, adresseAllouee, idx, nomMembre + "_ptrinit");
                            
                            for (size_t i = 0; i < arrayInit->getElements().size(); ++i) {
                                std::vector<llvm::Value*> indices = {
                                    builder.getInt32(0),
                                    builder.getInt32(static_cast<uint32_t>(i))
                                }; 
                                llvm::Value* ptrCase = builder.CreateGEP(typeArrayLLVM, membrePtr, indices, "ptr_case");
                                
                                INode* element = arrayInit->getElements()[i];
                                element->accept(this);
                                llvm::Value* expressionVal = _contextGenCode->getValeurTemporaire().getAdresse();
                                
                                llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(expressionVal, typeElement);
                                builder.CreateStore(valeurCastee, ptrCase);
                            }
                        }
                    }
                } else {
                    expressionInit->accept(this);
                    llvm::Value* valeurCalculee = _contextGenCode->getValeurTemporaire().getAdresse();

                    if (valeurCalculee != nullptr && infoClasse->getMemberIndices().find(nomMembre) != infoClasse->getMemberIndices().end()) {
                        unsigned int idx = infoClasse->getMemberIndices()[nomMembre];
                        
                        Token tokenMembre; tokenMembre.value = nomMembre;
                        Symbole modele = infoClasse->getRegistryVariable()->recupererVariables(tokenMembre);
                        llvm::Type* typeMembre = modele.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());

                        llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(valeurCalculee, typeMembre);
                        llvm::Value* membrePtr = builder.CreateStructGEP(typeCible, adresseAllouee, idx, nomMembre + "_ptrinit");
                        builder.CreateStore(valeurCastee, membrePtr);
                    }
                }
            }
        }
    }

    // Construction du builder avec arguments
    if (infoClasse != nullptr) {
        std::string nomBuilder = nodeNew->getNomType().value;
        if (infoClasse->getRegistryFunctionLocale()->existe(nomBuilder)) {
            const auto& symbolePtr = infoClasse->getRegistryFunctionLocale()->recuperer(nomBuilder);
            if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
                throw std::runtime_error("Error : SymboleFunctionLocale attendu");
            }
            const auto* symboleFunction = prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
            builder.CreateCall(symboleFunction->function, argsBuilder);
        }
    }

    _contextGenCode->modifierValeurTemporaire(Symbole(adresseAllouee, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), _contextGenCode->getValeurTemporaire().getType(), typeCible));
}