#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <llvm-18/llvm/IR/Value.h>

// Importation des noeuds
class NoeudScope;
class NoeudAffectationVariable;
class NoeudDeclarationVariable;
class NoeudVariable;
class NoeudAppelFonction;
class NoeudArgFonction;
class NoeudArgPassFonction;
class NoeudReturn;
class NoeudDeclarationFonction;
class Valeur;
class Instruction;
class Operation;

// Importation des classes 
class GenCodeAppelFonction;
class GenCodeArgFonction;
class GenCodeArgPassFonction;
class GenCodeDeclarationFonction;
class GenCodeReturn;
class GenCodeAffectationVariable;
class GenCodeDeclarationVariable;
class GenCodeValeur;
class GenCodeVariable;
class GenCodeOperation;
class GenCodeInstruction;
class GenCodeScope;

//Outils 
class RegistreVariable;
class LLVMBackend;

class VisiteurGeneralGenCode : public IVisiteur
{
private:
    llvm::Value* _valeurTemporaire = nullptr;

    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registre;
public:
    VisiteurGeneralGenCode(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registre);
    ~VisiteurGeneralGenCode();
    llvm::Value* getDerniereValeur() { return _valeurTemporaire; }
    
    void visiter(NoeudScope* NoeudScope) override;
    void visiter(NoeudAffectationVariable* noeudAffectationVariable) override;
    void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) override; 
    void visiter(NoeudVariable* noeudVariable) override;
    void visiter(NoeudAppelFonction* noeudAppelFonction) override;
    void visiter(NoeudArgPassFonction* noeudArgPassFonction) override;
    void visiter(NoeudReturn* noeudReturn) override;
    void visiter(NoeudArgFonction* noeudArgFonction) override;
    void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) override;
    void visiter(Valeur* valeur) override;
    void visiter(Instruction* instruction) override;
    void visiter(Operation* operation) override;
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
