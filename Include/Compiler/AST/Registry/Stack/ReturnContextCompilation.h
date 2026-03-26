#ifndef D5D5CC5E_96E0_4410_95A8_57E0E7660888
#define D5D5CC5E_96E0_4410_95A8_57E0E7660888

#include "Compiler/AST/Registry/Types/IType.h"
#include <stack>
class ReturnContextCompilation
{
private:
    std::stack<IType*> _context; 

public:

    ReturnContextCompilation()
    = default;

    ~ReturnContextCompilation()
    = default;

    ReturnContextCompilation(const ReturnContextCompilation&) = delete;
    auto operator=(const ReturnContextCompilation&) -> ReturnContextCompilation& = delete;

    ReturnContextCompilation(ReturnContextCompilation&&) = delete;
    auto operator=(ReturnContextCompilation&&) -> ReturnContextCompilation& = delete;

    auto getContext() -> IType*;
        
    void push(IType* token);
    void pop();

};

#endif /* D5D5CC5E_96E0_4410_95A8_57E0E7660888 */
