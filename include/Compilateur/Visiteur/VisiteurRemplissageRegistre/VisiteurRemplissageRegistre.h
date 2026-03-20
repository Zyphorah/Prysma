#ifndef E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#define E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"

// Ce visiteur a pour but de remplir les registres de fonction et de variable avant la génération du code, 
// Il va parcourir l'arbre syntaxique abstrait pour remplir les registres avec des valeur nullptr pour les fonction et les variables
// Cela permet d'avoir une première passe pour remplir les registres. 

class OrchestrateurInclude;

class VisiteurRemplissageRegistre : public VisiteurBaseGenerale
{
private:
    ContextGenCode* _contextGenCode;
    OrchestrateurInclude* _orchestrateur;
    
public:
    using VisiteurBaseGenerale::visiter;
    VisiteurRemplissageRegistre(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateur);
     ~VisiteurRemplissageRegistre() override;

    VisiteurRemplissageRegistre(const VisiteurRemplissageRegistre&) = delete;
    auto operator=(const VisiteurRemplissageRegistre&) -> VisiteurRemplissageRegistre& = delete;
    VisiteurRemplissageRegistre(VisiteurRemplissageRegistre&&) = delete;
    auto operator=(VisiteurRemplissageRegistre&&) -> VisiteurRemplissageRegistre& = delete;
    void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) override;
    void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) override;
    void visiter(NoeudInclude* noeudInclude) override;
    void visiter(NoeudClass* noeudClass) override;
};

#endif /* E5CE13D3_6823_4DDE_BB20_5311A0E477B6 */
