#ifndef EEA73704_BBAE_43AD_9799_F1F919E04250
#define EEA73704_BBAE_43AD_9799_F1F919E04250

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>

class IConstructeurArbre
{
public:
    virtual ~IConstructeurArbre() = default;
    virtual std::shared_ptr<INoeud> construire(std::vector<Token>& tokens) = 0;
};

#endif /* EEA73704_BBAE_43AD_9799_F1F919E04250 */
