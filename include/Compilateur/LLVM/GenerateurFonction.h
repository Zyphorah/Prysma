#ifndef D2944365_C1DD_41F8_A211_BFF33402A958
#define D2944365_C1DD_41F8_A211_BFF33402A958
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

class VisiteurGeneralGenCode;

class GestionFonction
{
    private:
        ContextGenCode* _contextGenCode;
        NoeudDeclarationFonction* _noeudDeclarationFonction;

        IVisiteur* _visiteurGeneralCodeGen;
       
        struct ArgumentsCodeGen
        {
            std::vector<llvm::Type*> argTypes;
            std::vector<NoeudArgFonction*> arguments;
        };
    
        ArgumentsCodeGen chargerArguments();
        
        // Étapes logiques de génération
        llvm::Function* creerFonction();
        void enregistrerFonction(llvm::Function* function);
        void initialiserContexte();
        void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen);
        void traiterCorpsFonction();
        void finaliserContexte();
        void genererBuiltInPrint(NoeudAppelFonction* noeudAppelFonction);


        // Appel de fonction
        void passArguments(NoeudAppelFonction* noeudAppelFonction);
        SymboleFonction obtenirFonction(const std::string& nomFonction);
        void genererAppelFonction(SymboleFonction symboleFonction);
      
    public: 

        void declarerFonction();
        void genererAppelFonction(NoeudAppelFonction* noeudAppelFonction);
        GestionFonction(ContextGenCode* _contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen);
        ~GestionFonction();
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
