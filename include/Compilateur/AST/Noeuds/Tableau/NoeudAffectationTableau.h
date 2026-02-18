#ifndef F277567C_57C1_46A6_AF10_6FD2159FCEFA
#define F277567C_57C1_46A6_AF10_6FD2159FCEFA

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm/IR/Instructions.h>
#include <memory>

class ContextGenCode;

class NoeudAffectationTableau : public INoeud
{
private:
    std::string _nom;
    std::shared_ptr<INoeud> _expressionIndex;
    std::shared_ptr<INoeud> _expression;
    Token _token;

public:
    NoeudAffectationTableau(const std::string& nom, std::shared_ptr<INoeud> expressionIndex, std::shared_ptr<INoeud> expression, Token token);
    ~NoeudAffectationTableau();

    void accept(IVisiteur* visiteur) override;

    const std::string& getNom() const { return _nom; }
    std::shared_ptr<INoeud>& getExpressionIndex() { return _expressionIndex; }
    std::shared_ptr<INoeud>& getExpression() { return _expression; }
    const Token& getToken() const { return _token; }
};

#endif /* F277567C_57C1_46A6_AF10_6FD2159FCEFA */
