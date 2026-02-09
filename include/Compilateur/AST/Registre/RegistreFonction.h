#ifndef F2141F07_2C85_4ADB_9BC9_A909EBD34394
#define F2141F07_2C85_4ADB_9BC9_A909EBD34394

#include "RegistreGeneric.h"
#include <llvm-18/llvm/IR/Function.h>
#include <string>

class RegistreFonction : public RegistreGeneric<std::string, llvm::Function*>
{

public:
    RegistreFonction() = default;
    ~RegistreFonction() = default;

};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
