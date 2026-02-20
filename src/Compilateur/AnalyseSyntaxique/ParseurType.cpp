#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>

ParseurType::ParseurType(RegistreType* registreType, IConstructeurArbre* constructeurArbre)
    : _registreType(registreType), _constructeurArbre(constructeurArbre)
{
}

IType* ParseurType::parser(std::vector<Token>& tokens, int& index)
{
    // Vérifier que le token courant est un type valide
    if (TokenCategories::TYPES.find(tokens[index].type) == TokenCategories::TYPES.end()) {
        throw ErreurCompilation("Erreur : type attendu", tokens[index].ligne, tokens[index].colonne);
    }

    // Récupérer le type LLVM depuis le registre et créer un TypeSimple
    llvm::Type* typeLLVM = _registreType->recuperer(tokens[index].type);
    IType* type = new TypeSimple(typeLLVM);
    index++;

    while (index < static_cast<int>(tokens.size()) && tokens[index].type == TOKEN_CROCHET_OUVERT) {
        index++; // Consommer le crochet ouvert

        INoeud* tailleEquation = _constructeurArbre->construire(tokens, index);

        if (index >= static_cast<int>(tokens.size()) || tokens[index].type != TOKEN_CROCHET_FERME) {
            throw ErreurCompilation("Erreur : ']' attendu après la taille du tableau", tokens[index].ligne, tokens[index].colonne);
        }
        index++; // Consommer le crochet fermant

        type = new TypeTableau(type, tailleEquation);
    }

    return type;
}
