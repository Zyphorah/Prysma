#ifndef DDD959AA_55F6_46B7_A433_8E073A1EACCE
#define DDD959AA_55F6_46B7_A433_8E073A1EACCE

#include <memory>
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"

class ParseurVariable
{

public: 
    static std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index); 
};

#endif /* DDD959AA_55F6_46B7_A433_8E073A1EACCE */
