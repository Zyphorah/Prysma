#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
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
    std::shared_ptr<IParser> ParentNoeud = _registreInstructions->recuperer(tokens[index].type);
    std::shared_ptr<INoeud> enfant = ParentNoeud->parser(tokens, index, std::make_shared<ConstructeurArbreInstruction>(*this));
    return enfant;
}