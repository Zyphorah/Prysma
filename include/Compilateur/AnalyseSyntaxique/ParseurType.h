#ifndef BA12D61E_9205_4816_94C1_379B8ABA63D3
#define BA12D61E_9205_4816_94C1_379B8ABA63D3

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>
#include <vector>

class ParseurType {
private:
    RegistreType* _registreType;
    IConstructeurArbre* _constructeurArbre;

public:
     ParseurType(RegistreType* registreType, IConstructeurArbre* constructeurArbre);
    ~ParseurType() = default;

    IType* parser(std::vector<Token>& tokens, int& index);
};

#endif /* BA12D61E_9205_4816_94C1_379B8ABA63D3 */
