#include "Compilateur/AST/Noeuds/Fonction/NoeudRetour.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>
#include <vector>
#include <llvm-18/llvm/IR/Type.h>

ParseurRetour::ParseurRetour(IConstructeurArbre* constructeurEquation)
    : _constructeurEquation(constructeurEquation)
{
}

ParseurRetour::~ParseurRetour()
{
}

std::shared_ptr<INoeud> ParseurRetour::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    if (constructeurArbre == nullptr) {
        throw ErreurCompilation("Erreur : IConstructeurArbre est null dans ParsingReturn", 1, 1);
    }
    
    consommer(tokens, index, TOKEN_RETOUR, "Erreur: ce n'est pas le bon token ! 'return'");

    std::shared_ptr<INoeud> valeurRetour = nullptr;

    if (index < (int)tokens.size() && tokens[index].type != TOKEN_POINT_VIRGULE) {
        if (_constructeurEquation != nullptr) {
            valeurRetour = _constructeurEquation->construire(tokens, index);
        } else {
            ParseurEquation parseurEquation(constructeurArbre);
            valeurRetour = parseurEquation.parser(tokens, index, constructeurArbre);
            return std::make_shared<NoeudRetour>(valeurRetour);
        }
    } else {
        consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur: point-virgule attendu après return");
    }
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin du return");

    return std::make_shared<NoeudRetour>(valeurRetour);
}
