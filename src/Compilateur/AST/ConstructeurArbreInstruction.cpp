#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include <memory>
#include <vector>


ConstructeurArbreInstruction::ConstructeurArbreInstruction()
= default;

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


std::shared_ptr<std::vector<INoeud>> ConstructeurArbreInstruction::obtenirTouteInstructionEnfant(const std::vector<Token>& tokensIntervalle)
{
	std::shared_ptr<std::vector<INoeud>> listeEnfants = std::make_shared<std::vector<INoeud>>();
	std::size_t indexLocal = 0;
	
	while (indexLocal < tokensIntervalle.size())
	{
		std::shared_ptr<INoeud> enfant = construire(const_cast<std::vector<Token>&>(tokensIntervalle));
		listeEnfants->push_back(*enfant);
		indexLocal++;
	}
	return listeEnfants;
}


std::shared_ptr<INoeud> ConstructeurArbreInstruction::construire(std::vector<Token>& tokens)
{
	// Registre d'instruction contenant une lambda de construction de nœud stockée dans un dictionnaire, retourne un nœud
	// noeuds = registreInstruction.recuperer(tokens[index].type);
	// Récupérer un vecteur de nœuds enfants
	auto enfants = obtenirTouteInstructionEnfant(tokens);
	// Ajouter les nœuds à l'arbre syntaxique abstrait
	// noeuds.ajouterInstruction(enfants);
	return nullptr;
}
