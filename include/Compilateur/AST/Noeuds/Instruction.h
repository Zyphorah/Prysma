#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include <vector>
#include <memory>


class Instruction: public IInstruction
{
protected: 
    std::vector<std::shared_ptr<INoeud>> enfants; 
public: 

    Instruction();
    virtual ~Instruction() = default;

    void ajouterInstruction(const std::vector<std::shared_ptr<INoeud>>& enfantsParam) override;
    
    llvm::Value* executerEnfants();
    
    const std::vector<std::shared_ptr<INoeud>>& getEnfants() const;
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
