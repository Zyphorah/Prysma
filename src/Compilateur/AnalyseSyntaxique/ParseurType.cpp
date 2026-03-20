#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/AST/Registre/Types/TypeComplexe.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <llvm/IR/Type.h>
#include <vector>

ParseurType::ParseurType(RegistreType* registreType, IConstructeurArbre* constructeurArbre)
    : _registreType(registreType), _constructeurArbre(constructeurArbre)
{
}

auto ParseurType::parser(std::vector<Token>& tokens, int& index) -> IType*
{
    // Vérifier que le token courant est un type valide
    if (!estType(tokens[static_cast<size_t>(index)].type)) {
        throw ErreurCompilation("Erreur : type attendu", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
    }

    IType* type = nullptr;
    
    if (tokens[static_cast<size_t>(index)].type == TOKEN_IDENTIFIANT) {
        type = _constructeurArbre->allouer<TypeComplexe>(tokens[static_cast<size_t>(index)].value);
    } else {
        llvm::Type* typeLLVM = _registreType->recuperer(tokens[static_cast<size_t>(index)].type);
        type = _constructeurArbre->allouer<TypeSimple>(typeLLVM);
    }
    index++;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_CROCHET_OUVERT) {
    
        index++; // Consommer le crochet ouvert

        INoeud* tailleEquation = nullptr;
        
        // Vérifier si la taille est spécifiée ou si les crochets sont vides
        if (tokens[static_cast<size_t>(index)].type != TOKEN_CROCHET_FERME) {
            tailleEquation = _constructeurArbre->construire(tokens, index);
        }

        if (index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != TOKEN_CROCHET_FERME) {
            throw ErreurCompilation("Erreur : ']' attendu après la taille du tableau", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
        }
        index++; // Consommer le crochet fermant

        type = _constructeurArbre->allouer<TypeTableau>(type, tailleEquation);
    }
    return type;
}
