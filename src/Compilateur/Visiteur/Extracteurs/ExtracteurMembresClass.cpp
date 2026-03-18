#include "Compilateur/Visiteur/Extracteurs/ExtracteurMembresClass.h"
#include "Compilateur/AST/AST_Genere.h"

void ExtracteurMembresClass::visiter(NoeudDeclarationFonction* node) { 
    methodes.push_back(node); 
}

void ExtracteurMembresClass::visiter(NoeudDeclarationVariable* node) { 
    variables.push_back(node); 
}

void ExtracteurMembresClass::visiter(NoeudClass* node) { 
    nomClass = node->getNomClass().value; 
}