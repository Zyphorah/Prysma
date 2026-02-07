#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParserAppelFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include <memory>


ParserAppelFonction::ParserAppelFonction()
{
}

ParserAppelFonction::~ParserAppelFonction()
{
    
}

//call nomFonction()
std::shared_ptr<INoeud> ParserAppelFonction::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) 
{

    consommer(tokens,index,TOKEN_CALL,"Erreur: ce n'est pas le token call, la fonction est invalid! 'call'");
    Token nomFonction = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: l'identifiant est invalid");
    std::shared_ptr<IInstruction> parent = make_shared<NoeudAppelFonction>(nomFonction);
    consommer(tokens,index,TOKEN_PAREN_OUVERTE, "Erreur: le token est invalide!'('");
    consommerEnfantCorps(tokens,index,parent,constructeurArbreInstruction,TOKEN_PAREN_FERMEE);
    consommer(tokens,index,TOKEN_PAREN_FERMEE, "Erreur: le token est invalide!')'");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Erreur: le token est invalide!';'");

    return parent; 
}