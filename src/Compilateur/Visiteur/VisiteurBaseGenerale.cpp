#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudRetour.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.h"

// Implémentations des méthodes visiter de base générale par défaut (ne font rien)
void VisiteurBaseGenerale::visiter(NoeudScope* NoeudScope)
{
    for (auto* instruction : NoeudScope->getEnfants()) 
    {
        if (instruction != nullptr) 
        {
            instruction->accept(this);
        }
    }
}
void VisiteurBaseGenerale::visiter(NoeudAffectationVariable* noeudAffectationVariable)
{
    noeudAffectationVariable->getExpression()->accept(this);
}

void VisiteurBaseGenerale::visiter(NoeudDeclarationVariable* noeudDeclarationVariable)
{
    if (noeudDeclarationVariable->getExpression() != nullptr)
    {
        noeudDeclarationVariable->getExpression()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudAppelFonction* noeudAppelFonction [[maybe_unused]])
{
    
}

void VisiteurBaseGenerale::visiter(NoeudRetour* noeudReturn)
{
    if (noeudReturn->getValeurRetour() != nullptr)
    {
        noeudReturn->getValeurRetour()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudArgFonction* noeudArgFonction [[maybe_unused]])
{
 
}

void VisiteurBaseGenerale::visiter(NoeudDeclarationFonction* noeudDeclarationFonction)
{
    for (auto* arg : noeudDeclarationFonction->getArguments())
    {
        arg->accept(this);
    }
    if (noeudDeclarationFonction->getCorps() != nullptr)
    {
        noeudDeclarationFonction->getCorps()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudOperation* noeudOperation)
{
    if (noeudOperation->getGauche() != nullptr)
    {
        noeudOperation->getGauche()->accept(this);
    }
    if (noeudOperation->getDroite() != nullptr)
    {
        noeudOperation->getDroite()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudLitteral* noeudLitteral [[maybe_unused]])
{
}

void VisiteurBaseGenerale::visiter(NoeudInstruction* instruction)
{
    for (auto* enfant : instruction->getEnfants())
    {
        enfant->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudUnRefVariable* noeudUnRefVariable [[maybe_unused]])
{
}

void VisiteurBaseGenerale::visiter(NoeudRefVariable* noeudRefVariable)
{
}

void VisiteurBaseGenerale::visiter(NoeudIf* noeudIf)
{
    if (noeudIf->getNoeudCondition() != nullptr)
    {
        noeudIf->getNoeudCondition()->accept(this);
    }
    if (noeudIf->getNoeudBlocIf() != nullptr)
    {
        noeudIf->getNoeudBlocIf()->accept(this);
    }
    if (noeudIf->getNoeudBlocElse() != nullptr)
    {
        noeudIf->getNoeudBlocElse()->accept(this);
    }
     if (noeudIf->getNoeudBlocEndif() != nullptr)
    {
        noeudIf->getNoeudBlocEndif()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudWhile* noeudWhile)
{
    if (noeudWhile->getNoeudCondition() != nullptr)
    {
        noeudWhile->getNoeudCondition()->accept(this);
    }
    if (noeudWhile->getNoeudBlocWhile() != nullptr)
    {
        noeudWhile->getNoeudBlocWhile()->accept(this);
    }
    if (noeudWhile->getNoeudBlocFinWhile() != nullptr)
    {
        noeudWhile->getNoeudBlocFinWhile()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudNegation* noeudNegation)
{
    if (noeudNegation->getOperande() != nullptr)
    {
        noeudNegation->getOperande()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudTableauInitialisation* noeudTableauInit)
{
    for (auto* element : noeudTableauInit->getElements())
    {
        element->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudAffectationTableau* noeudAffectationTableau)
{
    if (noeudAffectationTableau->getExpressionIndex() != nullptr)
    {
        noeudAffectationTableau->getExpressionIndex()->accept(this);
    }
    if (noeudAffectationTableau->getExpression() != nullptr)
    {
        noeudAffectationTableau->getExpression()->accept(this);
    }
}

void VisiteurBaseGenerale::visiter(NoeudLectureTableau* noeudLectureTableau)
{
    if (noeudLectureTableau->getIndexEquation() != nullptr)
    {
        noeudLectureTableau->getIndexEquation()->accept(this);
    }
}
