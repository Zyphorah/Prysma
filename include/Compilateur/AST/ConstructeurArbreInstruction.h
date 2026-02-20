#ifndef BDC39C44_6952_4793_8198_C083B106A089
#define BDC39C44_6952_4793_8198_C083B106A089

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"

class ConstructeurArbreInstruction : public IConstructeurArbre
{
private: 
    RegistreInstruction* _registreInstructions;
public: 

    ConstructeurArbreInstruction(RegistreInstruction* registreInstructions);
    ~ConstructeurArbreInstruction();

    INoeud* construire(std::vector<Token>& tokens) override;  
    INoeud* construire(std::vector<Token>& tokens, int& index) override;

};

#endif /* BDC39C44_6952_4793_8198_C083B106A089 */
