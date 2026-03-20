#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>

ConstructeurArbreInstruction::ConstructeurArbreInstruction(RegistreInstruction* registreInstructions, llvm::BumpPtrAllocator& arena)
    : _registreInstructions(registreInstructions), _arena(arena)
{
    
}

ConstructeurArbreInstruction::~ConstructeurArbreInstruction()
= default;

auto ConstructeurArbreInstruction::construire(std::vector<Token>& tokens, int& index) -> INoeud*
{
    if (!_registreInstructions->existe(tokens[static_cast<size_t>(index)].type)) {
        throw ErreurCompilation("Instruction inconnue : '" + tokens[static_cast<size_t>(index)].value + "'", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
    }
    IParseur* ParentNoeud = _registreInstructions->recuperer(tokens[static_cast<size_t>(index)].type);
    INoeud* enfant = ParentNoeud->parser(tokens, index);
    return enfant;
}


auto ConstructeurArbreInstruction::construire(std::vector<Token>& tokens) -> INoeud*
{
    int index = 0; 
    // On crée un conteneur global pour tout le fichier
    auto* programmeGlobal = allouer<NoeudInstruction>();
    
    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_EOF) {
        INoeud* enfant = construire(tokens, index);
        if (enfant != nullptr) {
            programmeGlobal->ajouterInstruction(enfant);
        }
    }
    return programmeGlobal;
}

auto ConstructeurArbreInstruction::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}