#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudReturn.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingReturn.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include <memory>
#include <utility>
#include <vector>
#include <llvm-18/llvm/IR/Type.h>

ParsingReturn::ParsingReturn()
{
}

ParsingReturn::~ParsingReturn()
{
}

std::shared_ptr<INoeud> ParsingReturn::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    if (constructeurArbreInstruction == nullptr) {
        throw std::runtime_error("Erreur : ConstructeurArbreInstruction est null dans ParsingReturn");
    }
    
    consommer(tokens, index, TOKEN_RETOUR, "Erreur: ce n'est pas le bon token ! 'return'");

    std::shared_ptr<INoeud> valeurRetour = nullptr;

    if (index < (int)tokens.size() && tokens[index].type != TOKEN_POINT_VIRGULE) {
        ParseurEquation parseurEquation;  
        valeurRetour = parseurEquation.parser(tokens, index, constructeurArbreInstruction);
    } else {
        consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur: point-virgule attendu après return");
    }

    return std::make_shared<NoeudReturn>(valeurRetour);
}
