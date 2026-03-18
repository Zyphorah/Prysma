#ifndef C2ADEE91_A0DA_4404_8AF5_5B1105A499EC
#define C2ADEE91_A0DA_4404_8AF5_5B1105A499EC

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <string>
#include <map>

struct Class
{
    RegistreFonctionLocale* registreFonctionLocale;
    RegistreVariable* registreVariable;

    llvm::GlobalVariable* vtable;
    
    // Pour calculer la taille automatique de l'objet avec llvm 
    llvm::StructType* structType;
    INoeud* parentHeritage;
    
    // Mapping des indices des membres pour la Passe 3
    // Clé : nom de la variable, Valeur : index dans la structure LLVM
    std::map<std::string, unsigned int> memberIndices;
    
    // Mapping des indices des méthodes dans la vtable
    std::map<std::string, unsigned int> methodIndices;
};
// TODO : supporter les mutex multi thread pour les classes Sinon le multi fichier n'est pas possible

class RegistreClass : public RegistreGeneric<std::string, Class*> 
{
public:
    RegistreClass() = default;
    virtual ~RegistreClass() = default;
};


#endif /* C2ADEE91_A0DA_4404_8AF5_5B1105A499EC */
