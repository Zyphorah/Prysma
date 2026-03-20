#ifndef D5B94044_819C_4C15_B528_AAE97CBCB264
#define D5B94044_819C_4C15_B528_AAE97CBCB264

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <string>

class ExtracteurTypeVariable {
private:
    ContextGenCode* _context;
public:
    explicit ExtracteurTypeVariable(ContextGenCode* context);
    auto extraire(llvm::Value* adresseMemoire) -> llvm::Type*;
};

class AdresseurVariable {
private:
    ContextGenCode* _context;
public:
    explicit AdresseurVariable(ContextGenCode* context);
    auto recupererAdresse(const std::string& nomVariable) -> Symbole;
};

class ChargeurVariable {
private:
    ContextGenCode* _context;
    AdresseurVariable _adresseur;
    ExtracteurTypeVariable _extracteur;
public:
    explicit ChargeurVariable(ContextGenCode* context);
    auto charger(const std::string& nomVariable) -> Symbole;
    auto chargerUnref(const std::string& nomVariable) -> Symbole;
};

class AllocateurVariable {
private:
    ContextGenCode* _context;
public:
    explicit AllocateurVariable(ContextGenCode* context);
    llvm::AllocaInst* allouer(llvm::Type* type, const std::string& nomVariable);
    void stocker(llvm::Value* valeur, llvm::AllocaInst* allocaInst);
};

class AffecteurVariable {
private:
    ContextGenCode* _context;
    ExtracteurTypeVariable _extracteur;
public:
    explicit AffecteurVariable(ContextGenCode* context);
    void affecter(llvm::Value* variableExistante, llvm::Value* valeur, llvm::Type* typeVariableLLVM = nullptr);
};

#endif /* D5B94044_819C_4C15_B528_AAE97CBCB264 */
