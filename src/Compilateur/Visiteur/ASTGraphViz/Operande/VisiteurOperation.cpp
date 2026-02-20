#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Type.h>
#include <memory>
#include <string>

void VisiteurGeneralGraphViz::visiter(NoeudOperation* noeud)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud(noeud->getToken().value);
    
    INoeud* gauche = noeud->getGauche();
    if (gauche) {
        gauche->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }

    INoeud* droite = noeud->getDroite();
    if (droite) {
        droite->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}

