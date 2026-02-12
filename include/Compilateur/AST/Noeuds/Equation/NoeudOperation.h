#ifndef ABF41CB0_250D_41D4_A3C0_36FF9372D9FF
#define ABF41CB0_250D_41D4_A3C0_36FF9372D9FF
#pragma once

#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>

class NoeudOperation : public IExpression {
private:
    TokenType typeOperation;
    std::shared_ptr<INoeud> gauche;
    std::shared_ptr<INoeud> droite;

public:
    explicit NoeudOperation(TokenType type);

    void setGauche(std::shared_ptr<INoeud> gauche_node) { gauche = std::move(gauche_node); }
    void setDroite(std::shared_ptr<INoeud> droite_node) { droite = std::move(droite_node); }

    std::shared_ptr<IExpression> ajouterExpression(std::shared_ptr<INoeud> gauche_node, std::shared_ptr<INoeud> droite_node) override {
        setGauche(std::move(gauche_node));
        setDroite(std::move(droite_node));
        return std::static_pointer_cast<IExpression>(std::make_shared<NoeudOperation>(*this));
    }

    void accept(IVisiteur* visiteur) override;

    friend class VisiteurGeneralGenCode;
};


#endif /* ABF41CB0_250D_41D4_A3C0_36FF9372D9FF */
