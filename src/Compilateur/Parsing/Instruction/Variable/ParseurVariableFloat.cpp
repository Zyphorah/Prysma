#include "Compilateur/Parsing/Instruction/Variable/ParseurVariableFloat.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include <cstddef>
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>
#include <utility>


ParseurVariableFloat::ParseurVariableFloat(std::shared_ptr<LLVMBackend> backend)
{
   _backend = std::move(backend);
}

ParseurVariableFloat::~ParseurVariableFloat()
{
}


// Exemple: var float teste = 34+435+4;
std::shared_ptr<INoeud> ParseurVariableFloat::parser(std::vector<Token>& tokens, int& index)
{
    //Vérifie et on consomme le début : "var float nom ="
    index++; // Passe 'var'
    index++; // Passe 'float' 

    std::string nomVariable = tokens[index].value;
    index++; // Passe le nom

    // Vérification du '='
    if (tokens[index].type != TOKEN_EGAL) {
        throw std::runtime_error("Erreur : '=' attendu après le nom de variable");
    }
    index++; // Passe le '='.
    
    // On cherche la fin de l'instruction (le point-virgule)
    int debutEquation = index;
    size_t finEquation = index;
    
    while (finEquation < tokens.size() && tokens[finEquation].type != TOKEN_POINT_VIRGULE) {
        finEquation++;
    }

    if (finEquation == tokens.size()) {
        throw std::runtime_error("Erreur : ';' manquant à la fin de la déclaration");
    }

    // Creer un sous vecteur qui contien seulement l'équation
    std::vector<Token> tokensEquation(tokens.begin() + debutEquation, tokens.begin() + finEquation);

    std::unique_ptr<FloatEquationBuilder> floatEquationBuilder = std::make_unique<FloatEquationBuilder>(_backend->getContext());
    
    std::shared_ptr<INoeud> expression = floatEquationBuilder->builderArbreEquationFloat(tokensEquation);

    
    shared_ptr<NoeudDeclaration> noeudDeclaration = std::make_shared<NoeudDeclaration>(
        _backend, 
        nomVariable, 
        _backend->getBuilder().getFloatTy(), 
        expression->genCode() 
    );

    // Mise à jour de l'index globale 
    index = finEquation + 1;

    return noeudDeclaration;
}