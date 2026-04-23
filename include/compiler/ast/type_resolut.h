#ifndef CCAC10B3_2D13_4991_9530_BAAAE1B22F83
#define CCAC10B3_2D13_4991_9530_BAAAE1B22F83

class IType;

// TODO : je ne suis pas certain que ce soit la meilleure solution, mais ça permet de résoudre le problème actuel
// Jinja2 sans refaire une refactorisation
// Faire de l'indirection mémoire mais j'ai toujours mon arbre syntaxique abstrait immuable

class TypeResolut {
private:
    IType* typeEvaluated = nullptr;
public:
    [[nodiscard]] auto getTypeEvaluated() const -> IType* { return typeEvaluated; }
    void setTypeEvaluated(IType* type) { typeEvaluated = type; }
};

#endif /* CCAC10B3_2D13_4991_9530_BAAAE1B22F83 */
