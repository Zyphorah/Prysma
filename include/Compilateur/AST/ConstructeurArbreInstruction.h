#ifndef BDC39C44_6952_4793_8198_C083B106A089
#define BDC39C44_6952_4793_8198_C083B106A089

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"

class ConstructeurArbreInstruction : public IConstructeurArbre
{
private: 
    int avancerFinInstruction(const std::vector<Token>& tokens, int indexActuel);
    std::shared_ptr<std::vector<INoeud>> obtenirTouteInstructionEnfant(const std::vector<Token>& tokensIntervalle);
    
public: 

    ConstructeurArbreInstruction();
    ~ConstructeurArbreInstruction();

    std::shared_ptr<INoeud> construire(std::vector<Token>& tokens);
};

#endif /* BDC39C44_6952_4793_8198_C083B106A089 */
