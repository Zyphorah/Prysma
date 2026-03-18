#include "Compilateur/Visiteur/Extracteurs/ExtracteurArgFonction.h"
#include "Compilateur/AST/AST_Genere.h"

void ExtracteurArgFonction::visiter(NoeudArgFonction* noeud) {
    arg = noeud;
}