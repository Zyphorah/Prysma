#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingDeclarationFonction.h"
#include <memory>
#include <vector>
// Exemple : def int main(){}
std::shared_ptr<INoeud> ParsingDeclarationFonction::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    // Construire le noeud parent 
    std::shared_ptr<NoeudDeclarationFonction> parent = std::make_shared<NoeudDeclarationFonction>();

    consommer(tokens, index, TOKEN_FONCTION, "Erreur: ce n'est pas le bon token ! 'def'");

    Token tokenTypeRetour = tokens[index];
    
    if (tokenTypeRetour.type == TOKEN_TYPE_INT || 
        tokenTypeRetour.type == TOKEN_TYPE_FLOAT || 
        tokenTypeRetour.type == TOKEN_TYPE_BOOL || 
        tokenTypeRetour.type == TOKEN_TYPE_VOID) {
        
        index++; 
    } else {
        throw std::runtime_error("Erreur: Type de retour attendu (int, float, void...)");
    }

    consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant invalide, ce dois être un nom de fonction ");

    // Manger tout les paramètres

    while(tokens[index].type != TOKEN_PAREN_FERMEE)
    {
        /*
            std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
            std::vector<shared_ptr<INoeud>> enfants; 
            enfants.push_back(enfant);
            parent->ajouterInstruction(enfants);
        */
    }

    // Manger tout ce qui ce trouve dans les accolade 
    while(tokens[index].type != TOKEN_ACCOLADE_FERMEE)
    {
        std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
        std::vector<shared_ptr<INoeud>> enfants; 
        enfants.push_back(enfant);
        parent->ajouterInstruction(enfants);
    }

    return parent; 
}