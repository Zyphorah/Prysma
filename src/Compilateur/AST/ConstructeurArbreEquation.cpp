#include "AST/ConstructeurArbreEquation.h"

#include <utility>
#include <llvm/IR/Constants.h>
#include "AST/Noeuds/Interfaces/INoeud.h"
#include "AST/Noeuds/Operande/Valeur.h"
#include "AST/Noeuds/Interfaces/IExpression.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    std::shared_ptr<RegistreSymbole> registreSymbole,
    IGestionnaireParenthese* gestionnaireParenthese,
    llvm::LLVMContext& context)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(std::move(registreSymbole)), 
      _gestionnaireParenthese(gestionnaireParenthese),
      _context(context) {}

std::shared_ptr<IExpression> ConstructeurArbreEquation::construire(std::string equation) {
    equation = _gestionnaireParenthese->enleverParenthesesEnglobantes(equation);
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);
    
    if (indice == -1) {
        double valeurDouble = std::stod(equation);
        llvm::Value* valeurLLVM = llvm::ConstantFP::get(_context, llvm::APFloat(valeurDouble));
        return std::make_shared<Valeur>(valeurLLVM);
    }
    
    std::shared_ptr<INoeud> noeud = _registreSymbole->recupererNoeud(equation[indice]);
    std::shared_ptr<IExpression> gauche = construire(equation.substr(0, indice));
    std::shared_ptr<IExpression> droite = construire(equation.substr(indice + 1));
    
    noeud->ajouterExpression(gauche, droite);
    return noeud;
}
