#ifndef D8229B93_7089_41D9_9BA2_4433A4DF8D65
#define D8229B93_7089_41D9_9BA2_4433A4DF8D65

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>


class NoeudNegation : public INoeud {
private:
    std::shared_ptr<INoeud> operande;
    Token operateur;

public:
    explicit NoeudNegation(const Token& operateur, std::shared_ptr<INoeud> expr)
        : operande(std::move(expr)), operateur(operateur) {}

    void accept(IVisiteur* visiteur) override;

    const std::shared_ptr<INoeud>& getOperande() const { return operande; }
    const Token& getOperateur() const { return operateur; }
};


#endif /* D8229B93_7089_41D9_9BA2_4433A4DF8D65 */
