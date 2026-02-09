#ifndef D2944365_C1DD_41F8_A211_BFF33402A958
#define D2944365_C1DD_41F8_A211_BFF33402A958
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <memory>

class VisiteurGeneralGenCode;

class GenerateurFonction
{
    private:
        std::shared_ptr<ContextGenCode> _contextGenCode;
        NoeudDeclarationFonction* _noeudDeclarationFonction;
        const std::vector<shared_ptr<INoeud>>& enfants;

        IVisiteur* _visiteurGeneralCodeGen;
       
        struct ArgumentsCodeGen
        {
            std::vector<llvm::Type*> argTypes;
            std::vector<NoeudArgFonction*> arguments;
        };
    
        ArgumentsCodeGen chargerArguments();
        
        // Étapes logiques de génération
        llvm::Function* creerFonction(llvm::Type* typeDeRetour, const ArgumentsCodeGen& argumentsCodeGen);
        void enregistrerFonction(llvm::Function* function);
        void initialiserContexte();
        void traiterArguments(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen);
        void traiterCorpsFonction();
        void finaliserContexte();
      
    public: 

        void declarerFonction();
        GenerateurFonction(shared_ptr<ContextGenCode> _contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen);
        ~GenerateurFonction();
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
