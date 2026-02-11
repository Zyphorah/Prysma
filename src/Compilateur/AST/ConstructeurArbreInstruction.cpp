#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>
#include <vector>


ConstructeurArbreInstruction::ConstructeurArbreInstruction(std::shared_ptr<RegistreInstruction> registreInstructions)
{
	_registreInstructions = std::move(registreInstructions);
}

ConstructeurArbreInstruction::~ConstructeurArbreInstruction()
{
}

std::shared_ptr<INoeud> ConstructeurArbreInstruction::construire(std::vector<Token>& tokens, int& index)
{
    if (!_registreInstructions->existe(tokens[index].type)) {
        throw ErreurCompilation("Instruction inconnue : '" + tokens[index].value + "'", tokens[index].ligne, tokens[index].colonne);
    }
    std::shared_ptr<IParseur> ParentNoeud = _registreInstructions->recuperer(tokens[index].type);
    std::shared_ptr<INoeud> enfant = ParentNoeud->parser(tokens, index, this);
    return enfant;
}

std::shared_ptr<INoeud> ConstructeurArbreInstruction::construire(std::vector<Token>& tokens)
{
    int index = 0; 
    return construire(tokens, index);
}