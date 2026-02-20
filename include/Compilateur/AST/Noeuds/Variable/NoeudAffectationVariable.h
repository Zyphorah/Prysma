#ifndef F529C0E5_B051_4DF3_9DB0_8987D960AAF6
#define F529C0E5_B051_4DF3_9DB0_8987D960AAF6

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm/IR/Instructions.h>
#include <memory>

class ContextGenCode;

class NoeudAffectationVariable : public INoeud
{
private:
    std::string _nom;
    INoeud* _expression;
    Token _token; 

public:
    NoeudAffectationVariable(const std::string& nom, INoeud* expression, Token token);
    ~NoeudAffectationVariable();

    void accept(IVisiteur* visiteur) override;

    const std::string& getNom() const { return _nom; }
    INoeud* getExpression() { return _expression; }
    const Token& getToken() const { return _token; }
};

#endif /* F529C0E5_B051_4DF3_9DB0_8987D960AAF6 */
