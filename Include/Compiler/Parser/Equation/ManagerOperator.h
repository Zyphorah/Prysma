#ifndef FF112DD1_03F3_41D3_8B8F_5E7E64C5467A
#define FF112DD1_03F3_41D3_8B8F_5E7E64C5467A

#include "Compiler/Parser/Equation/Interfaces/IManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>

/**
 * @class ManagerOperateur
 * @brief Maillon de la chaîne de responsabilité pour les opérateurs
 * Implémente le pattern Chain of Responsibility
 */
class ManagerOperateur : public IManagerOperateur {
private:
    IManagerOperateur* _suivant;
    IManagerParenthese* _managerParenthese;
    TokenType _tokenType;

public:
    /**
     * @brief Builder
     * @param tokenType Le type de token représentant cet opérateur
     */
    explicit ManagerOperateur(TokenType tokenType);
    
    /**
     * @brief Définit le manager de gestion des parenthèses
     * @param managerParenthese Manager de parenthèses
     */
    void definirManagerParenthese(IManagerParenthese* managerParenthese);
    
    /**
     * @brief Définit le prochain maillon de la chaîne
     * @param suivant Le prochain manager
     */
    void definirSuivant(IManagerOperateur* suivant) override;
    
    /**
     * @brief Trouve cet opérateur dans l'équation
     * @param equation L'équation à analyser
     * @return L'index de l'opérateur, ou -1
     */
    [[nodiscard]] virtual int chercherOperateur(const std::vector<Token>& equation) const;
    
    /**
     * @brief Traite la recherche de cet opérateur ou délègue au suivant
     * @param equation L'équation à traiter
     * @return L'index de l'opérateur trouvé, ou -1
     */
    auto traiter(const std::vector<Token>& equation) -> int override;
};


#endif /* FF112DD1_03F3_41D3_8B8F_5E7E64C5467A */
