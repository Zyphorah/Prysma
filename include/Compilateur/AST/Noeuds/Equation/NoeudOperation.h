#ifndef ABF41CB0_250D_41D4_A3C0_36FF9372D9FF
#define ABF41CB0_250D_41D4_A3C0_36FF9372D9FF
#pragma once

#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>
#include <string>

class NoeudOperation : public IExpression {
private:
    Token _operation;
    INoeud* gauche;
    INoeud* droite;

public:
    explicit NoeudOperation(Token operation);

    void setGauche(INoeud* gauche_node) { gauche = gauche_node; }
    void setDroite(INoeud* droite_node) { droite = droite_node; }

    IExpression* ajouterExpression(INoeud* gauche_node, INoeud* droite_node) override {
        setGauche(gauche_node);
        setDroite(droite_node);
        return this;
    }

    void accept(IVisiteur* visiteur) override;

    const Token& getToken() const { return _operation; }
    INoeud* getGauche() const { return gauche; }
    INoeud* getDroite() const { return droite; }
};


#endif /* ABF41CB0_250D_41D4_A3C0_36FF9372D9FF */
