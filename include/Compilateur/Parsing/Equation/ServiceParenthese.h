#ifndef DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F
#define DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F

#include "Compilateur/Parsing/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/AST/Noeuds/Operande/RegistreSymbole.h"
#include <memory>
#include <string>

class ServiceParenthese : public IGestionnaireParenthese {
private:
    std::shared_ptr<RegistreSymbole> _registreSymbole;
    
    /**
     * @brief Vérifie si une paire de parenthèses est englobante
     * @param equation L'équation à vérifier
     * @return true si les parenthèses englobent toute l'expression
     */
    bool estEnglobante(const std::string& equation) const;
    
    /**
     * @brief Vérifie si un opérateur à une position donnée est unaire
     * @param equation L'équation
     * @param indice L'indice de l'opérateur
     * @return true si c'est un signe unaire
     */
    bool estSigneUnaire(const std::string& equation, int indice) const;

public:
    /**
     * @brief Constructeur
     * @param registreSymbole Registre des opérateurs
     */
    explicit ServiceParenthese(std::shared_ptr<RegistreSymbole> registreSymbole);
    
    /**
     * @brief Enlève les parenthèses qui englobent toute l'expression
     * @param equation L'équation à traiter
     * @return L'équation sans parenthèses englobantes
     */
    std::string enleverParenthesesEnglobantes(std::string equation) override;
    
    /**
     * @brief Trouve le dernier opérateur au niveau de parenthèses zéro
     * @param equation L'équation à analyser
     * @param operateur Le caractère opérateur à chercher
     * @return L'index de l'opérateur, ou -1 si non trouvé
     */
    int trouverDernierAuNiveauZero(const std::string& equation, char operateur) override;
};


#endif /* DA3AD58B_D7B7_44D3_AC56_E1DDCD78F54F */
