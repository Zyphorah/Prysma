#ifndef DC6C556F_6309_4430_98EB_911BFF855216
#define DC6C556F_6309_4430_98EB_911BFF855216

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <llvm-18/llvm/IR/Value.h>

class NoeudRetour : public INoeud
{
private:
    INoeud* _valeurRetour;

public:
    explicit NoeudRetour(INoeud* valeurRetour);
    ~NoeudRetour() = default;

    void accept(IVisiteur* visiteur) override;

    INoeud* getValeurRetour() const { return _valeurRetour; }
};

#endif /* DC6C556F_6309_4430_98EB_911BFF855216 */
