#ifndef FF112DD1_03F3_41D3_8B8F_5E7E64C5467A
#define FF112DD1_03F3_41D3_8B8F_5E7E64C5467A

#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireOperateur.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>

/**
 * @class GestionnaireOperateur
 * @brief Maillon de la chaîne de responsabilité pour les opérateurs
 * Implémente le pattern Chain of Responsibility
 */
class GestionnaireOperateur : public IGestionnaireOperateur {
private:
    IGestionnaireOperateur* _suivant;
    IGestionnaireParenthese* _gestionnaireParenthese;
    TokenType _tokenType;

public:
    /**
     * @brief Constructeur
     * @param tokenType Le type de token représentant cet opérateur
     */
    explicit GestionnaireOperateur(TokenType tokenType);
    
    /**
     * @brief Définit le gestionnaire de gestion des parenthèses
     * @param gestionnaireParenthese Gestionnaire de parenthèses
     */
    void definirGestionnaireParenthese(IGestionnaireParenthese* gestionnaireParenthese);
    
    /**
     * @brief Définit le prochain maillon de la chaîne
     * @param suivant Le prochain gestionnaire
     */
    void definirSuivant(IGestionnaireOperateur* suivant) override;
    
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
