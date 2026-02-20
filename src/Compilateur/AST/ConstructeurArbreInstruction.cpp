#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/GestionnaireErreur.h"
#include <vector>


ConstructeurArbreInstruction::ConstructeurArbreInstruction(RegistreInstruction* registreInstructions)
{
	_registreInstructions = registreInstructions;
}

ConstructeurArbreInstruction::~ConstructeurArbreInstruction()
{
}

INoeud* ConstructeurArbreInstruction::construire(std::vector<Token>& tokens, int& index)
{
    if (!_registreInstructions->existe(tokens[index].type)) {
        throw ErreurCompilation("Instruction inconnue : '" + tokens[index].value + "'", tokens[index].ligne, tokens[index].colonne);
    }
    IParseur* ParentNoeud = _registreInstructions->recuperer(tokens[index].type);
    INoeud* enfant = ParentNoeud->parser(tokens, index);
    return enfant;
}

INoeud* ConstructeurArbreInstruction::construire(std::vector<Token>& tokens)
{
    int index = 0; 
    return construire(tokens, index);
}