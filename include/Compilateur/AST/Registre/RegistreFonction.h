#ifndef F2141F07_2C85_4ADB_9BC9_A909EBD34394
#define F2141F07_2C85_4ADB_9BC9_A909EBD34394

#include "RegistreGeneric.h"
#include <llvm-18/llvm/IR/Function.h>
#include <string>

class RegistreFonction : public RegistreGeneric<std::string, llvm::Function*>
{

public:
    RegistreFonction() = default;
    ~RegistreFonction() = default;

    void ajouter(const std::string& nom, llvm::Function* fonction) {
        RegistreGeneric::enregistrer(nom,fonction);
    }

    llvm::Function* obtenir(const std::string& nom){
        return  RegistreGeneric::recuperer(nom);
    }

    [[nodiscard]] bool existe(const std::string& nom) const {
        return RegistreFonction::existe(nom);
    }
};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
