#include "Compilateur/Visiteur/VisiteurRemplissageCorpsClass/VisiteurRemplissageCorpsClass.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include "Compilateur/Visiteur/Extracteurs/ExtracteurMembresClass.h"
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
                                                      const std::vector<NoeudDeclarationFonction*>& listMethodeParent)
{
    // Construire la vtable en alignant les méthodes du parent aux mêmes positions
    std::vector<llvm::Constant*> vtableElements;
    
    // Ajouter les pointeurs de fonction du parent à la vtable, dans le même ordre
    for (NoeudDeclarationFonction* declarationMethodeParent : listMethodeParent) {
        std::string nomMethodeParent = declarationMethodeParent->getNom();
        
        // Chercher la méthode correspondante dans la classe courante
        llvm::Function* functionImpl = nullptr;
        if (classInfo->registreFonctionLocale->existe(nomMethodeParent)) {
            const auto& symbole = classInfo->registreFonctionLocale->recuperer(nomMethodeParent);
            auto* symboleLocal = static_cast<SymboleFonctionLocale*>(symbole.get());
            functionImpl = symboleLocal->fonction;
        }
        
        // Si trouvée, ajouter le pointeur de fonction à la vtable
        if (functionImpl != nullptr) {
            vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                functionImpl,
                llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->backend->getContext()), 0)
            ));
            classInfo->methodIndices[nomMethodeParent] = vtableElements.size() - 1;
        }
    }
    
    // Ajouter les méthodes supplémentaires de la classe courante qui ne sont pas dans le parent
    auto clesMethodes = classInfo->registreFonctionLocale->obtenirCles();
    for (const auto& cle : clesMethodes) {
        const std::string& nomMethode = cle;
        bool estDansParent = false;
        
        // Vérifier si cette méthode est dans le parent
        for (NoeudDeclarationFonction* declarationMethodeParent : listMethodeParent) {
            if (declarationMethodeParent->getNom() == nomMethode) {
                estDansParent = true;
                break;
            }
        }
        
        // Si ce n'est pas une méthode du parent, l'ajouter à la fin de la vtable
        if (!estDansParent) {
            const auto& symbole = classInfo->registreFonctionLocale->recuperer(nomMethode);
            auto* symboleLocal = static_cast<SymboleFonctionLocale*>(symbole.get());
            if (symboleLocal->fonction != nullptr) {
                vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                    symboleLocal->fonction,
                    llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->backend->getContext()), 0)
                ));
                classInfo->methodIndices[nomMethode] = vtableElements.size() - 1;
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

    ExtracteurMembresClass extracteurClass;
    for (INoeud* membre : noeudClass->getListMembres()) {
        if (membre != nullptr) {
            membre->accept(&extracteurClass);
        }
    }

    // 1. Récupération de l'objet class 
    Class* classInfo = _contextGenCode->registreClass->recuperer(nomClass);

    // 2. Résolution récursive de l'héritage 
    INoeud* ParentHeritage = classInfo->parentHeritage;
    if(ParentHeritage != nullptr)
    {
        ParentHeritage->accept(this);
    }

    // 3. Initialisation du layout ( vecteur de type) 
    std::vector<llvm::Type*> elementsCorpsClass;

    // Faire le vpointeur à l'adresse zéro 
    elementsCorpsClass.push_back(llvm::PointerType::get(
        llvm::Type::getInt8Ty(
        _contextGenCode->backend->getContext()),
         0
        ));


    // 4. Héritage des champs du parent et construction de la VTable
    if(ParentHeritage != nullptr)
    {
        ExtracteurMembresClass extracteurParent;
        ParentHeritage->accept(&extracteurParent);
        std::string nomParent = extracteurParent.nomClass;

        Class* classParentInfo = _contextGenCode->registreClass->recuperer(nomParent);

        // On extrait les méthodes du parent pour les binder à la même position de la vtable
        auto clesMethodesParent = classParentInfo->registreFonctionLocale->obtenirCles();
      
        std::vector<NoeudDeclarationFonction*> listMethodeParent;
        for (const auto& cle : clesMethodesParent) {
            const auto& symbole = classParentInfo->registreFonctionLocale->recuperer(cle);
            auto* symboleLocal = static_cast<SymboleFonctionLocale*>(symbole.get());
            if (symboleLocal->noeud != nullptr) {
                ExtracteurMembresClass methodeExtr;
                symboleLocal->noeud->accept(&methodeExtr);
                if (!methodeExtr.methodes.empty()) {
                    listMethodeParent.push_back(methodeExtr.methodes[0]);
                }
            }
        }

        // On regarde si il y a les méthodes du parent dans la classe courante, si non c'est une erreur 
        for(NoeudDeclarationFonction* methodeParent : listMethodeParent)
        {
            bool trouve = false;
            for(NoeudDeclarationFonction* methodeClasse : extracteurClass.methodes)
            {
                if(methodeClasse->getNom() == methodeParent->getNom())
                {
                    trouve = true;
                    break;
                }
            }

            if(!trouve)
            {
                std::string errorMsg = "La classe " + nomClass + " doit implémenter la méthode ";
                errorMsg += methodeParent->getNom();
                errorMsg += " héritée de " + nomParent;
                throw std::runtime_error(errorMsg);
            }
        }

        // Construire la vtable
        construireVTable(classInfo, nomClass, listMethodeParent);
    }
    else
    {
        // Classe sans parent : construire la VTable avec ses propres méthodes
        construireVTable(classInfo, nomClass, extracteurClass.methodes);
    }
    
    // 5. Extraction des variables de l'AST
    // L'index commence à 1 car l'index 0 est réservé au vtable pointer
    unsigned int indexCourant = 1;
    
    // Parcourir les membres pour récupérer les variables dans l'ordre de déclaration
    for(NoeudDeclarationVariable* declarationVariable : extracteurClass.variables)
    {
        IType* itype = declarationVariable->getType();
        llvm::Type* typeVariable = itype->genererTypeLLVM(_contextGenCode->backend->getContext());
        if (typeVariable != nullptr) {
            elementsCorpsClass.push_back(typeVariable);
            // Enregistrer l'index pour la Passe 3
            classInfo->memberIndices[declarationVariable->getNom()] = indexCourant;
            indexCourant++;
        }
    }

    // 6. Scellage de la structure (setBody)
    if (classInfo->structType != nullptr) {
        classInfo->structType->setBody(elementsCorpsClass);
    }
}