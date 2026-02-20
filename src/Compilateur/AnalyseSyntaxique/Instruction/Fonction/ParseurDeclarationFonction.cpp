#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>
#include <utility>
#include <vector>

ParseurDeclarationFonction::ParseurDeclarationFonction(IConstructeurArbre* constructeurArbreInstruction, ParseurType* parseurType)
    : _constructeurArbreInstruction(constructeurArbreInstruction), _parseurType(parseurType)
{}

ParseurDeclarationFonction::~ParseurDeclarationFonction()
{
}

INoeud* ParseurDeclarationFonction::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_FONCTION, "Erreur: ce n'est pas le bon token ! 'fn'");

    Token tokenTypeRetour = tokens[index];
    IType* typeRetour = _parseurType->parser(tokens, index);
    
    Token tokenNomFonction = tokens[index];
    std::string nomFonction = tokenNomFonction.value;
    consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant invalide, ce dois être un nom de fonction ");

    IInstruction* parent = new NoeudDeclarationFonction(nomFonction, typeRetour);

    // Manger les parenthèses ouvertes
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: ce n'est pas une parenthèse ouverte '('");
    
    // Manger tout les paramètres
    consommerEnfantCorps(tokens,index,parent,_constructeurArbreInstruction,TOKEN_PAREN_FERMEE);

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ce n'est pas une parenthèse fermée ')'");
    // Manger tout ce qui ce trouve dans les accolades
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur: ce n'est pas une accolade ouverte '{' ");
    consommerEnfantCorps(tokens,index,parent,_constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Erreur: ce n'est pas une accolade fermée '}'");

    return parent; 
}