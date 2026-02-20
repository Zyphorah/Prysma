#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieString.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <llvm-18/llvm/Support/Allocator.h>
#include <vector>
#include <string>
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"


StrategieString::StrategieString(llvm::BumpPtrAllocator& allocator) 
: _allocator(allocator)
{}

StrategieString::~StrategieString()
{}


// "chaineCaractere"
INoeud* StrategieString::construire(std::vector<Token>& equation) 
{
    std::vector<INoeud*> elementsString;

    int index = 0; 
    consommer(equation,index,TOKEN_GUILLEMET,"Erreur: une chaine de caractere doit commencer par un guillemet");

    Token chaine = consommer(equation,index,TOKEN_IDENTIFIANT,"Erreur: une chaine de caractere doit etre composee de caracteres alphanumeriques");

    consommer(equation,index,TOKEN_GUILLEMET,"Erreur: une chaine de caractere doit finir par un guillemet");

    // Transformer le texte en code ascii
    for(size_t i = 0; i < chaine.value.size(); i++)
    {
        int ascii = chaine.value[i];
        Token token; 
        token.type = TOKEN_LIT_INT;
        token.value = std::to_string(ascii);
        token.ligne = chaine.ligne;
        token.colonne = chaine.colonne;
        elementsString.push_back(new (_allocator.Allocate<NoeudLitteral>()) NoeudLitteral(token));
    }
    Token tokenZero; 
    tokenZero.type = TOKEN_LIT_INT;
    tokenZero.value = "0";
    tokenZero.ligne = chaine.ligne;
    tokenZero.colonne = chaine.colonne;
    elementsString.push_back(new (_allocator.Allocate<NoeudLitteral>()) NoeudLitteral(tokenZero));
   
    return new (_allocator.Allocate<NoeudTableauInitialisation>()) NoeudTableauInitialisation(elementsString);
}