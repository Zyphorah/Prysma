#include "Compilateur/Visiteur/VisiteurRemplissageCorpsClass/VisiteurRemplissageCorpsClass.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include <llvm-18/llvm/IR/Metadata.h>
#include <string>
#include <vector>

VisiteurRemplissageCoprsClass::VisiteurRemplissageCoprsClass(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

VisiteurRemplissageCoprsClass::~VisiteurRemplissageCoprsClass()
{}

void VisiteurRemplissageCoprsClass::construireVTable(Class* classInfo, const std::string& nomClass, 
                                                      const std::vector<INoeud*>& listMethodeParent)
{
    // Construire la vtable en alignant les méthodes du parent aux mêmes positions
    std::vector<llvm::Constant*> vtableElements;
    
    // Ajouter les pointeurs de fonction du parent à la vtable, dans le même ordre
    for (INoeud* methodeParent : listMethodeParent) {
        auto* declarationMethodeParent = static_cast<NoeudDeclarationFonction*>(methodeParent);
        std::string nomMethodeParent = declarationMethodeParent->getNom();
        
        // Chercher la méthode correspondante dans la classe courante
        llvm::Function* functionImpl = nullptr;
        auto methodesClasses = classInfo->registreFonctionLocale->getElements();
        for (const auto& pair : methodesClasses) {
            if (pair.first == nomMethodeParent) {
                auto* symbole = static_cast<SymboleFonctionLocale*>(pair.second.get());
                functionImpl = symbole->fonction;
                break;
            }
        }
        
        // Si trouvée, ajouter le pointeur de fonction à la vtable
        if (functionImpl != nullptr) {
            vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                functionImpl,
                llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->backend->getContext()), 0)
            ));
        }
    }
    
    // Ajouter les méthodes supplémentaires de la classe courante qui ne sont pas dans le parent
    auto methodesClasses = classInfo->registreFonctionLocale->getElements();
    for (const auto& pair : methodesClasses) {
        std::string nomMethode = pair.first;
        bool estDansParent = false;
        
        // Vérifier si cette méthode est dans le parent
        for (INoeud* methodeParent : listMethodeParent) {
            auto* declarationMethodeParent = static_cast<NoeudDeclarationFonction*>(methodeParent);
            if (declarationMethodeParent->getNom() == nomMethode) {
                estDansParent = true;
                break;
            }
        }
        
        // Si ce n'est pas une méthode du parent, l'ajouter à la fin de la vtable
        if (!estDansParent) {
            auto* symbole = static_cast<SymboleFonctionLocale*>(pair.second.get());
            if (symbole->fonction != nullptr) {
                vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                    symbole->fonction,
                    llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->backend->getContext()), 0)
                ));
            }
        }
    }
    
    // Créer un array constant pour la vtable
    llvm::ArrayType* vtableType = llvm::ArrayType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->backend->getContext()), 0),
        vtableElements.size()
    );
    llvm::Constant* vtableInitializer = llvm::ConstantArray::get(vtableType, vtableElements);
    
    // Créer une variable globale pour la vtable
    llvm::Module* module = &(_contextGenCode->backend->getModule());
    classInfo->vtable = new llvm::GlobalVariable(
        *module,
        vtableType,
        true,  
        llvm::GlobalValue::LinkageTypes::InternalLinkage,
        vtableInitializer,
        "vtable_" + nomClass
    );
}

void VisiteurRemplissageCoprsClass::visiter(NoeudClass* noeudClass)
{
    std::string nomClass = noeudClass->getNomClass().value;

    // 1. Récupération de l'objet class 
    Class* classInfo = _contextGenCode->registreClass->recuperer(nomClass);

    // 2. Résolution récursive de l'héritage 
    INoeud* ParentHeritage = classInfo->parentHeritage;
    ParentHeritage->accept(this);

    // 3. Initialisation du layout ( vecteur de type) 
    
    std::vector<llvm::Type*> elementsCorpsClass;

    // Faire le vpointeur à l'adresse zéro 
    elementsCorpsClass.push_back(llvm::PointerType::get(
        llvm::Type::getInt8Ty(
        _contextGenCode->backend->getContext()),
         0
        ));


    // 4. Héritage des champs du parent 
   
    if(ParentHeritage != nullptr)
    {
        if(ParentHeritage->getTypeGenere() != NoeudTypeGenere::Class)
        {
            throw std::runtime_error("Le parent d'une classe doit être une classe. Erreur dans la classe : " + nomClass);
        }

        std::string nomParent = static_cast<NoeudClass*>(ParentHeritage)->getNomClass().value;
        Class* classParentInfo = _contextGenCode->registreClass->recuperer(nomParent);

        // On extrait les méthodes du parent pour les binder à la même position de la vtable
        auto methodesParent = classParentInfo->registreFonctionLocale->getElements();
      
        std::vector<INoeud*> listMethodeParent;
        for (const auto& pair : methodesParent) {
            auto* symbole = static_cast<SymboleFonctionLocale*>(pair.second.get());
            if (symbole->noeud != nullptr) {
                listMethodeParent.push_back(symbole->noeud);
            }
        }


        // On regarde si il y a les méthodes du parent dans la classe courante, si non c'est une erreur 
        for(INoeud* methodeParent : listMethodeParent)
        {
            bool trouve = false;
            for(INoeud* methodeClasse : noeudClass->getListFonction())
            {
                if(methodeClasse->getTypeGenere() == NoeudTypeGenere::DeclarationFonction)
                {
                    auto* declarationMethodeClasse = static_cast<NoeudDeclarationFonction*>(methodeClasse);
                    auto* declarationMethodeParent = static_cast<NoeudDeclarationFonction*>(methodeParent);
                    if(declarationMethodeClasse->getNom() == declarationMethodeParent->getNom())
                    {
                        trouve = true;
                        break;
                    }
                }
            }

            if(!trouve)
            {
                std::string errorMsg = "La classe " + nomClass + " doit implémenter la méthode ";
                errorMsg += static_cast<NoeudDeclarationFonction*>(methodeParent)->getNom();
                errorMsg += " héritée de " + nomParent;
                throw std::runtime_error(errorMsg);
            }
        }

        // Construire la vtable
        construireVTable(classInfo, nomClass, listMethodeParent);
    }

    // 5. Extraction des variables de l'AST
    // Parcourir les variables publiques
    for(INoeud* variable : noeudClass->getListVariable())
    {
        if(variable->getTypeGenere() != NoeudTypeGenere::DeclarationVariable)
        {
            std::string errorMsg = "Tous les membres de la section variable d'une classe doivent être des déclarations de variable. ";
            errorMsg += "Erreur dans la classe : " + nomClass;
            throw std::runtime_error(errorMsg);
        }

        auto* declarationVariable = static_cast<NoeudDeclarationVariable*>(variable);
        IType* itype = declarationVariable->getType();
        llvm::Type* typeVariable = itype->genererTypeLLVM(_contextGenCode->backend->getContext());
        if (typeVariable != nullptr) {
            elementsCorpsClass.push_back(typeVariable);
        }
    }
    
    // Parcourir les variables privées
    for(INoeud* variable : noeudClass->getListVariablePrive())
    {
        if(variable->getTypeGenere() != NoeudTypeGenere::DeclarationVariable)
        {
            std::string errorMsg = "Tous les membres de la section variable d'une classe doivent être des déclarations de variable. ";
            errorMsg += "Erreur dans la classe : " + nomClass;
            throw std::runtime_error(errorMsg);
        }

        auto* declarationVariable = static_cast<NoeudDeclarationVariable*>(variable);
        IType* itype = declarationVariable->getType();
        llvm::Type* typeVariable = itype->genererTypeLLVM(_contextGenCode->backend->getContext());
        if (typeVariable != nullptr) {
            elementsCorpsClass.push_back(typeVariable);
        }
    }
    
    // Parcourir les variables protégées
    for(INoeud* variable : noeudClass->getListVariableProtected())
    {
        if(variable->getTypeGenere() != NoeudTypeGenere::DeclarationVariable)
        {
            std::string errorMsg = "Tous les membres de la section variable d'une classe doivent être des déclarations de variable. ";
            errorMsg += "Erreur dans la classe : " + nomClass;
            throw std::runtime_error(errorMsg);
        }

        auto* declarationVariable = static_cast<NoeudDeclarationVariable*>(variable);
        IType* itype = declarationVariable->getType();
        llvm::Type* typeVariable = itype->genererTypeLLVM(_contextGenCode->backend->getContext());
        if (typeVariable != nullptr) {
            elementsCorpsClass.push_back(typeVariable);
        }
    }

    // 6. Scellage de la structure (setBody)
    if (classInfo->structType != nullptr) {
        classInfo->structType->setBody(elementsCorpsClass);
    }
}