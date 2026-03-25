#ifndef D5B94044_819C_4C15_B528_AAE97CBCB264
#define D5B94044_819C_4C15_B528_AAE97CBCB264

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <string>

class ExtractorTypeVariable {
private:
    ContextGenCode* _context;
public:
    explicit ExtractorTypeVariable(ContextGenCode* context);
    auto extraire(llvm::Value* adresseMemory) -> llvm::Type*;
};

class AdresseurVariable {
private:
    ContextGenCode* _context;
public:
    explicit AdresseurVariable(ContextGenCode* context);
    auto recupererAdresse(const std::string& nomVariable) -> Symbole;
};

class LoadurVariable {
private:
    ContextGenCode* _context;
    AdresseurVariable _adresseur;
    ExtractorTypeVariable _extracteur;
public:
    explicit LoadurVariable(ContextGenCode* context);
    auto loadr(const std::string& nomVariable) -> Symbole;
    auto loadrUnref(const std::string& nomVariable) -> Symbole;
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
    ExtractorTypeVariable _extracteur;
public:
    explicit AffecteurVariable(ContextGenCode* context);
    void affecter(llvm::Value* variableExistante, llvm::Value* valeur, llvm::Type* typeVariableLLVM = nullptr);
};

#endif /* D5B94044_819C_4C15_B528_AAE97CBCB264 */
