#ifndef BA12D61E_9205_4816_94C1_379B8ABA63D3
#define BA12D61E_9205_4816_94C1_379B8ABA63D3

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class ParseurType {
private:
    RegistreType* _registreType;
    IConstructeurArbre* _constructeurArbre;

public:
     ParseurType(RegistreType* registreType, IConstructeurArbre* constructeurArbre);
    ~ParseurType() = default;

    ParseurType(const ParseurType&) = delete;
    auto operator=(const ParseurType&) -> ParseurType& = delete;
    ParseurType(ParseurType&&) = delete;
    auto operator=(ParseurType&&) -> ParseurType& = delete;
    auto parser(std::vector<Token>& tokens, int& index) -> IType*;
};

#endif /* BA12D61E_9205_4816_94C1_379B8ABA63D3 */
