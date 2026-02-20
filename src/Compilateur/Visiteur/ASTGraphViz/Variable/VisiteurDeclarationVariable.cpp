#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGraphViz::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Declaration: " + noeudDeclarationVariable->getNom());

    if (noeudDeclarationVariable->getExpression() != nullptr) {
        noeudDeclarationVariable->getExpression()->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}

