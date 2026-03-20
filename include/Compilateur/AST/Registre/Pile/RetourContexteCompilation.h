#ifndef D5D5CC5E_96E0_4410_95A8_57E0E7660888
#define D5D5CC5E_96E0_4410_95A8_57E0E7660888

#include "Compilateur/AST/Registre/Types/IType.h"
#include <stack>
class RetourContexteCompilation
{
private:
    std::stack<IType*> _contexte; 

public:

    RetourContexteCompilation()
    = default;

    ~RetourContexteCompilation()
    = default;

    RetourContexteCompilation(const RetourContexteCompilation&) = delete;
    auto operator=(const RetourContexteCompilation&) -> RetourContexteCompilation& = delete;

    RetourContexteCompilation(RetourContexteCompilation&&) = delete;
    auto operator=(RetourContexteCompilation&&) -> RetourContexteCompilation& = delete;

    auto recupererContext() -> IType*;
        
    void piler(IType* token);
    void depiler();

};

#endif /* D5D5CC5E_96E0_4410_95A8_57E0E7660888 */
