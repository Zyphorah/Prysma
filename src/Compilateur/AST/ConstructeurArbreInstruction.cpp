#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include <memory>
#include <vector>


ConstructeurArbreInstruction::ConstructeurArbreInstruction(const std::shared_ptr<RegistreInstruction> registreInstructions)
{
	_registreInstructions = registreInstructions;
}


int ConstructeurArbreInstruction::avancerFinInstruction(const std::vector<Token>& tokens, int indexActuel)
{
	int idx = indexActuel;
	while (tokens[idx].type != TOKEN_POINT_VIRGULE)
	{
		indexActuel++;
		idx++;
	}
	return indexActuel;
}


std::vector<std::shared_ptr<INoeud>> ConstructeurArbreInstruction::obtenirTouteInstructionEnfant(const std::vector<Token>& tokensIntervalle)
{
	std::vector<std::shared_ptr<INoeud>> listeEnfants = std::vector<std::shared_ptr<INoeud>>();
	std::size_t indexLocal = 0;
	
	while (indexLocal < tokensIntervalle.size())
	{
		std::shared_ptr<INoeud> enfant = construire(const_cast<std::vector<Token>&>(tokensIntervalle), indexLocal);
		listeEnfants.push_back(enfant);
		indexLocal++;
	}
	return listeEnfants;
}


std::shared_ptr<INoeud> ConstructeurArbreInstruction::construire(std::vector<Token>& tokens, int index)
{
	
    std::shared_ptr<IInstruction> ParentNoeud = _registreInstructions->recuperer(tokens[index].type);
    
    std::vector<std::shared_ptr<INoeud>> enfants = obtenirTouteInstructionEnfant(tokens);
    
    ParentNoeud->ajouterInstruction(enfants);
    
    return ParentNoeud;
}