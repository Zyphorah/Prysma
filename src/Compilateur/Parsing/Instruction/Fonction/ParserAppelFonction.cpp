#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParserAppelFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include <memory>


ParserAppelFonction::ParserAppelFonction(std::shared_ptr<RegistreFonction> registreFonction, std::shared_ptr<LLVMBackend> backend)
{
    _registreFonction = std::move(registreFonction);
    _backend = std::move(backend);
}

ParserAppelFonction::~ParserAppelFonction()
{
    
}

//call nomFonction()
std::shared_ptr<INoeud> ParserAppelFonction::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) 
{
    consommer(tokens,index,TOKEN_CALL,"Erreur: ce n'est pas le token call, la fonction est invalid! 'call'");
    Token nomFonction = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: l'identifiant est invalid");
    std::shared_ptr<IInstruction> parent = make_shared<NoeudAppelFonction>(nomFonction,_registreFonction, _backend);
    consommer(tokens,index,TOKEN_PAREN_OUVERTE, "Erreur: le token est invalide!'('");
    consommerEnfantCorps(tokens,index,parent,constructeurArbreInstruction,TOKEN_PAREN_FERMEE);
    consommer(tokens,index,TOKEN_PAREN_FERMEE, "Erreur: le token est invalide!')'");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Erreur: le token est invalide!';'");

    return parent; 
}