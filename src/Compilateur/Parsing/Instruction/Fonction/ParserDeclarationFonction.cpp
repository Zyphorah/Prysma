#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingDeclarationFonction.h"
#include <memory>
#include <vector>

ParsingDeclarationFonction::ParsingDeclarationFonction()
{}

ParsingDeclarationFonction::~ParsingDeclarationFonction()
{
}

std::shared_ptr<INoeud> ParsingDeclarationFonction::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    if (constructeurArbreInstruction == nullptr) {
        throw std::runtime_error("Erreur : ConstructeurArbreInstruction est null dans ParsingDeclarationFonction");
    }
    
    consommer(tokens, index, TOKEN_FONCTION, "Erreur: ce n'est pas le bon token ! 'fn'");

    Token tokenTypeRetour = tokens[index];
    TokenType typeRetour = tokenTypeRetour.type;
    
    if (TokenCategories::TYPES.find(typeRetour) != TokenCategories::TYPES.end()) {
        index++; 
    } else {
        throw std::runtime_error("Erreur: Type de retour attendu (int, float, void...)");
    }

    Token tokenNomFonction = tokens[index];
    std::string nomFonction = tokenNomFonction.value;
    consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant invalide, ce dois être un nom de fonction ");

    std::shared_ptr<IInstruction> parent = std::make_shared<NoeudDeclarationFonction>(nomFonction, typeRetour);

    // Manger les parenthèses ouvertes
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: ce n'est pas une parenthèse ouverte '('");
    
    // Manger tout les paramètres
    consommerEnfantCorps(tokens,index,parent,constructeurArbreInstruction,TOKEN_PAREN_FERMEE);

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ce n'est pas une parenthèse fermée ')'");
    // Manger tout ce qui ce trouve dans les accolades
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur: ce n'est pas une accolade ouverte '{' ");
    consommerEnfantCorps(tokens,index,parent,constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Erreur: ce n'est pas une accolade fermée '}'");

    return parent; 
}