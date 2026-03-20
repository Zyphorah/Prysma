#ifndef DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F
#define DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F

#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class RegistreSymbole;

class ServiceParenthese : public IGestionnaireParenthese {
private:
    RegistreSymbole* _registreSymbole;  
    /**
     * @brief Vérifie si une paire de parenthèses est englobante
     * @param equation L'équation à vérifier
     * @return true si les parenthèses englobent toute l'expression
     */
    static auto estEnglobante(const vector<Token>& equation) -> bool;
    
    /**
     * @brief Vérifie si un opérateur à une position donnée est unaire
     * @param equation L'équation
     * @param indice L'indice de l'opérateur
     * @return true si c'est un signe unaire
     */
    [[nodiscard]] auto estSigneUnaire(const vector<Token>& equation, int indice) const -> bool;

public:
    /**
     * @brief Constructeur
     * @param registreSymbole Registre des opérateurs
     */
    explicit ServiceParenthese(RegistreSymbole* registreSymbole);
    
    /**
     * @brief Enlève les parenthèses qui englobent toute l'expression
     * @param equation L'équation à traiter
     * @return L'équation sans parenthèses englobantes
     */
    auto enleverParenthesesEnglobantes(const std::vector<Token>& equation) -> std::vector<Token> override;
    
    /**
     * @brief Trouve le dernier opérateur au niveau de parenthèses zéro
     * @param equation L'équation à analyser
     * @param operateur Le caractère opérateur à chercher
     * @return L'index de l'opérateur, ou -1 si non trouvé
     */
    auto trouverDernierAuNiveauZero(const vector<Token>& equation, Token operateur) -> int override;
};


#endif /* DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F */
