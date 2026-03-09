#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <llvm-18/llvm/Support/Allocator.h>
#include <llvm/Support/TargetSelect.h>
#include "Compilateur/Lexer/TokenType.h"
#include "catch.hpp"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/AST_Genere.h"

// Stratégies d'équation
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieString.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"

// Parseurs d'instructions
#include "Compilateur/AnalyseSyntaxique/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurAppelFonction.h"
#include "Compilateur/AnalyseSyntaxique/ParseurDeclarationVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurUnRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/ParseurIf.h"
#include "Compilateur/AnalyseSyntaxique/ParseurWhile.h"
#include "Compilateur/AnalyseSyntaxique/ParseurInclude.h"

using namespace std;




struct EnvironnementAST {
    llvm::BumpPtrAllocator arena;
    std::unique_ptr<RegistreInstruction> registreInstruction;
    std::unique_ptr<RegistreStrategieEquation> registreStrategieEquation;
    std::unique_ptr<RegistreType> registreType;
    std::unique_ptr<RegistreVariable> registreVariable;

    ConstructeurArbreInstruction* constructeurArbre = nullptr;
    ConstructeurEquationFlottante* constructeurEquation = nullptr;
    ParseurType* parseurType = nullptr;
    ContextParseur* contextParseur = nullptr;

    EnvironnementAST() {
    
        registreInstruction = std::make_unique<RegistreInstruction>();
        registreStrategieEquation = std::make_unique<RegistreStrategieEquation>();
        registreType = std::make_unique<RegistreType>();
        registreVariable = std::make_unique<RegistreVariable>();

        // Constructeur d'arbre d'instruction 
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmismatched-new-delete"
        constructeurArbre = new (arena) ConstructeurArbreInstruction(registreInstruction.get(), arena);
        #pragma GCC diagnostic pop

        //  Strategie d'équation 
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmismatched-new-delete"
        constructeurEquation = new (arena) ConstructeurEquationFlottante(constructeurArbre, registreStrategieEquation.get(), arena);
        #pragma GCC diagnostic pop

        parseurType = new (arena.Allocate<ParseurType>()) ParseurType(registreType.get(), constructeurEquation->recupererConstructeurArbre());

        // Créer le ContextParseur
        contextParseur = new (arena.Allocate<ContextParseur>()) ContextParseur(
            constructeurEquation->recupererConstructeurArbre(),
            constructeurArbre,
            parseurType,
            arena,
            registreVariable.get(),
            registreType.get()
        );

        auto* stratLitInt = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_INT, stratLitInt);

        auto* stratLitFloat = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_FLOAT, stratLitFloat);

        auto* stratLitBool = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_BOLEEN, stratLitBool);

        auto* stratIdent = new (arena.Allocate<StrategieIdentifiant>()) StrategieIdentifiant(constructeurEquation);
        registreStrategieEquation->enregistrer(TOKEN_IDENTIFIANT, stratIdent);

        auto* stratRef = new (arena.Allocate<StrategieRef>()) StrategieRef(arena);
        registreStrategieEquation->enregistrer(TOKEN_REF, stratRef);

        auto* stratUnRef = new (arena.Allocate<StrategieUnRef>()) StrategieUnRef(arena);
        registreStrategieEquation->enregistrer(TOKEN_UNREF, stratUnRef);

        auto* stratNeg = new (arena.Allocate<StrategieNegation>()) StrategieNegation(constructeurEquation);
        registreStrategieEquation->enregistrer(TOKEN_NON, stratNeg);

        auto* stratTab = new (arena.Allocate<StrategieTableauInitialisation>()) StrategieTableauInitialisation(constructeurEquation->recupererConstructeurArbre());
        registreStrategieEquation->enregistrer(TOKEN_CROCHET_OUVERT, stratTab);

        auto* stratString = new (arena.Allocate<StrategieString>()) StrategieString(arena);
        registreStrategieEquation->enregistrer(TOKEN_GUILLEMET, stratString);

        auto* stratCall = new (arena.Allocate<StrategieAppelFonction>()) StrategieAppelFonction(*contextParseur);
        registreStrategieEquation->enregistrer(TOKEN_CALL, stratCall);

        // Parseurs d'instructions
        auto* parsFonc = new (arena.Allocate<ParseurDeclarationFonction>()) ParseurDeclarationFonction(*contextParseur);
        registreInstruction->enregistrer(TOKEN_FONCTION, parsFonc);

        auto* parsAff = new (arena.Allocate<ParseurAffectationVariable>()) ParseurAffectationVariable(*contextParseur);
        registreInstruction->enregistrer(TOKEN_AFF, parsAff);

        auto* parsDec = new (arena.Allocate<ParseurDeclarationVariable>()) ParseurDeclarationVariable(*contextParseur);
        registreInstruction->enregistrer(TOKEN_DEC, parsDec);

        auto* parsCall = new (arena.Allocate<ParseurAppelFonction>()) ParseurAppelFonction(*contextParseur);
        registreInstruction->enregistrer(TOKEN_CALL, parsCall);

        auto* parsRet = new (arena.Allocate<ParseurRetour>()) ParseurRetour(*contextParseur);
        registreInstruction->enregistrer(TOKEN_RETOUR, parsRet);

        auto* parsArg = new (arena.Allocate<ParseurArgFonction>()) ParseurArgFonction(*contextParseur);
        registreInstruction->enregistrer(TOKEN_ARG, parsArg);

        auto* parsUnRef = new (arena.Allocate<ParseurUnRefVariable>()) ParseurUnRefVariable(*contextParseur);
        registreInstruction->enregistrer(TOKEN_UNREF, parsUnRef);

        auto* parsRefVar = new (arena.Allocate<ParseurRefVariable>()) ParseurRefVariable(*contextParseur);
        registreInstruction->enregistrer(TOKEN_REF, parsRefVar);

        auto* parsIf = new (arena.Allocate<ParseurIf>()) ParseurIf(*contextParseur);
        registreInstruction->enregistrer(TOKEN_SI, parsIf);

        auto* parsWhile = new (arena.Allocate<ParseurWhile>()) ParseurWhile(*contextParseur);
        registreInstruction->enregistrer(TOKEN_TANT_QUE, parsWhile);

        auto* parsInclude = new (arena.Allocate<ParseurInclude>()) ParseurInclude(*contextParseur);
        registreInstruction->enregistrer(TOKEN_INCLUDE, parsInclude);
    }
};


/// Construit un arbre d'équation à partir d'une string de code source.
INoeud* construireEquationDepuisString(EnvironnementAST& env, const std::string& code) {
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenizer(code);

    // Retirer le token EOF ajouté par le Lexer
    if (!tokens.empty() && tokens.back().type == TOKEN_EOF) {
        tokens.pop_back();
    }

    return env.constructeurEquation->construire(tokens);
}

/// Construit un arbre d'instructions à partir d'une string de code source.
INoeud* construireArbreDepuisString(EnvironnementAST& env, const std::string& code) {
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenizer(code);
    return env.constructeurArbre->construire(tokens);
}

template<typename TypeAttendu, typename TypeEntrer>

TypeAttendu* verifierTypeEtCaster(TypeEntrer* noeud) {
    auto* noeudCast = dynamic_cast<TypeAttendu*>(noeud);
    REQUIRE(noeudCast != nullptr);
    return noeudCast;
}


template<auto getter, typename TypeAttendu, typename TypeValeur, auto accessor = nullptr>
struct matcherNoeud
{
    TypeValeur valeurAttendue;
    
    matcherNoeud(TypeValeur p_valeurAttendue) : valeurAttendue(std::move(p_valeurAttendue)) {}

    template<typename typeBase>
    void operator()(typeBase* noeud) {
       auto* noeudCast = verifierTypeEtCaster<TypeAttendu>(noeud);
       if constexpr (accessor == nullptr) {
           REQUIRE((noeudCast->*getter)() == valeurAttendue);
       } else {
           REQUIRE(((noeudCast->*getter)().*accessor) == valeurAttendue);
       }
    }
};

// Utilisation de la technique fonctor
template<typename TypeCible, auto getter, typename TypeValeur, auto accessor, typename Gauche, typename Droite>
struct matcherGeneralBinaire
{
    Gauche verificateurGauche;
    Droite verificateurDroite;
    TypeValeur valeurAttendue;

    matcherGeneralBinaire(const TypeValeur& p_valeur, Gauche p_noeudGaucheAttendu, Droite p_noeudDroiteAttendu) 
        : verificateurGauche(std::move(p_noeudGaucheAttendu)), verificateurDroite(std::move(p_noeudDroiteAttendu)), valeurAttendue(p_valeur) {}

    template<typename typeBase>
    void operator()(typeBase* noeud) {
        auto* noeudOperation = verifierTypeEtCaster<TypeCible>(noeud);
        
        if constexpr (accessor == nullptr) {
            REQUIRE((noeudOperation->*getter)() == valeurAttendue);
        } else {
            REQUIRE(((noeudOperation->*getter)().*accessor) == valeurAttendue);
        }
        verificateurDroite(noeudOperation->getDroite());
        verificateurGauche(noeudOperation->getGauche());
    }
};

// Matcher pour une liste d'enfant d'un noeud instruction ou autre
template<typename TypeCible, auto getter, typename... Matchers>
struct matcherListsEnfant 
{
    std::tuple<Matchers...> verificateurs;

    matcherListsEnfant(Matchers... p_verificateurs) : verificateurs(std::make_tuple(std::move(p_verificateurs)...)) {}

    template<typename typeBase>
    void operator()(typeBase* noeud) {
        auto* noeudCast = verifierTypeEtCaster<TypeCible>(noeud);

        // Je ne dois pas faire que get enfant car il peux y avoir noeudIf noeudWhile
        const auto& enfants = (noeudCast->*getter)();
        REQUIRE(enfants.size() == sizeof...(Matchers));
        verifierEnfants(enfants, std::index_sequence_for<Matchers...>{});
    }
};


// Faire les helper pour éviter d'avoir beaucoup de syntaxe dans les tests template 
auto Litteral(const std::string& valeur) {
    return matcherNoeud<&NoeudLitteral::getToken, NoeudLitteral, std::string, &Token::value>(valeur);
}

// Helper pour les opérations
auto operateur() {
    return [](const std::string& type, auto gauche, auto droite) {
        return matcherGeneralBinaire<NoeudOperation, &NoeudOperation::getToken, std::string, &Token::value, decltype(gauche), decltype(droite)>(
            type, gauche, droite
        );
    };
}

// Helper pour les listes d'enfant
template<typename T, auto Method, typename M>
struct matcherPropriete {
    M m;
    matcherPropriete(M m) : m(std::move(m)) {}
    void operator()(INoeud* n) {
        auto* t = verifierTypeEtCaster<T>(n);
        m((t->*Method)()); 
    }
};

// matcher utilitaire pour appliquer plusieurs matcher sur le même noeud
template<typename T, typename... Ms>
struct matcherCombine {
    std::tuple<Ms...> ms;
    matcherCombine(Ms... ms) : ms(std::make_tuple(std::move(ms)...)) {}
    void operator()(INoeud* n) {
        auto* t = verifierTypeEtCaster<T>(n);
        // Applique chaque matcher au nœud casté
        std::apply([&](auto&&... args) { (args(t), ...); }, ms);
    }
};

// C'est un fichier de test exclusif à l'arbre syntaxique abstrait première partie du test sera de vérifier la construction de l'arbre 
// d'equation et de tester des operations de base comme l'addition, la soustraction, la multiplication et la division.

TEST_CASE("Construction Arbre Equation Simple", "[AST]")
{
    cout<< "\nTest 1 - Construction d'un arbre pour l'equation '1 + 10 * 50'" << endl;
    
    EnvironnementAST env;
    INoeud* arbre = construireEquationDepuisString(env, "1 + 10 * 50");

    auto verificateur = 
    operateur()(
        "+", 
        Litteral("1"), 
        operateur()(
            "*", 
            Litteral("10"), 
            Litteral("50")
        ));
    
    verificateur(arbre);
}

TEST_CASE("Constructeur Arbre equation priorite", "[AST]")
{
    cout << "\nTest 2 - Construction d'un arbre avec les regles Prysma" << endl;
    EnvironnementAST env;
    INoeud* arbre = construireEquationDepuisString(env, "40 / 2 + 10 - 5 * 3");
    
    auto verificateur = 
    operateur()(
        "+", 
        operateur()(
            "/", 
            Litteral("40"), 
            Litteral("2")
        ), 
        operateur()(
            "-", 
            Litteral("10"), 
            operateur()(
                "*", 
                Litteral("5"), 
                Litteral("3")
            )
        )
    );
    verificateur(arbre);
}

TEST_CASE("Constructeur Arbre equation profondeur parenthèse", "[AST]")
{
    cout << "\nTest 3 - Construction d'un arbre avec des parenthèses" << endl;
    EnvironnementAST env;
    INoeud* arbre = construireEquationDepuisString(env, "(((40/2 +10)+ 5 * 3)+10)");

    auto verificateur = 
    operateur()(
        "+", 
        operateur()(
            "+", 
            operateur()(
                "+", 
                operateur()(
                    "/", 
                    Litteral("40"), 
                    Litteral("2")
                ), 
                Litteral("10")
            ), 
            operateur()(
                "*", 
                Litteral("5"), 
                Litteral("3")
            )
        ), 
        Litteral("10")
    );

    verificateur(arbre);
}

// Début des tests pour l'arbre d'instruction 

// Test de branchement if simple avec else
TEST_CASE("Construction Arbre If simple avec else", "[AST][Branch]")
{
    cout << "\nTest 4 - Arbre if/else simple" << endl;
    EnvironnementAST env;

    // Code Prysma : if avec condition et deux branches
    INoeud* arbre = construireArbreDepuisString(env,
        "if (a > 5) { aff x = 1; } else { aff x = 2; }"
    );
     
    REQUIRE(arbre != nullptr);

    // 1. Racine = NoeudInstruction qui contient le if
    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);
    REQUIRE(racine->getEnfants().size() == 1);

    // 2. Premier enfant = NoeudIf
    auto* noeudIf = dynamic_cast<NoeudIf*>(racine->getEnfants()[0]);
    REQUIRE(noeudIf != nullptr);

    // 3. Condition pas nulle
    REQUIRE(noeudIf->getNoeudCondition() != nullptr);

    // 4. Vérifier condition : opération '>'
    auto* condition = dynamic_cast<NoeudOperation*>(noeudIf->getNoeudCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_PLUS_GRAND);

    // 5. Bloc if existe et contient 1 instruction
    auto* blocIf = dynamic_cast<NoeudInstruction*>(noeudIf->getNoeudBlocIf());
    REQUIRE(blocIf != nullptr);
    REQUIRE(blocIf->getEnfants().size() == 1);

    // 6. Bloc else existe et contient 1 instruction
    auto* blocElse = dynamic_cast<NoeudInstruction*>(noeudIf->getNoeudBlocElse());
    REQUIRE(blocElse != nullptr);
    REQUIRE(blocElse->getEnfants().size() == 1);

    // 7. Bloc endif existe (noeud de sortie)
    REQUIRE(noeudIf->getNoeudBlocEndif() != nullptr);
}

// Test if sans else
TEST_CASE("Construction Arbre If sans else", "[AST][Branch]")
{
    cout << "\nTest 5 - Arbre if sans else" << endl;
    EnvironnementAST env;

    INoeud* arbre = construireArbreDepuisString(env,
        "if (a == 10) { aff y = 42; }"
    );

    REQUIRE(arbre != nullptr);

    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);

    // 1. NoeudIf
    auto* noeudIf = dynamic_cast<NoeudIf*>(racine->getEnfants()[0]);
    REQUIRE(noeudIf != nullptr);

    // 2. Condition == 
    auto* condition = dynamic_cast<NoeudOperation*>(noeudIf->getNoeudCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_EGAL_EGAL);

    // 3. Bloc if a 1 enfant
    auto* blocIf = dynamic_cast<NoeudInstruction*>(noeudIf->getNoeudBlocIf());
    REQUIRE(blocIf != nullptr);
    REQUIRE(blocIf->getEnfants().size() == 1);

    // 4. Pas de else
    REQUIRE(noeudIf->getNoeudBlocElse() == nullptr);

    // 5. Endif existe quand même
    REQUIRE(noeudIf->getNoeudBlocEndif() != nullptr);
}

// Test if avec condition logique && 
TEST_CASE("Construction Arbre If condition logique ET", "[AST][Branch]")
{
    cout << "\nTest 6 - Arbre if avec &&" << endl;
    EnvironnementAST env;

    INoeud* arbre = construireArbreDepuisString(env,
        "if (a > 1 && b < 10) { aff z = 0; }"
    );

    REQUIRE(arbre != nullptr);

    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);

    auto* noeudIf = dynamic_cast<NoeudIf*>(racine->getEnfants()[0]);
    REQUIRE(noeudIf != nullptr);

    // Condition racine = &&
    auto* condition = dynamic_cast<NoeudOperation*>(noeudIf->getNoeudCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_ET);

    // Gauche du && = '>'
    auto* gauche = dynamic_cast<NoeudOperation*>(condition->getGauche());
    REQUIRE(gauche != nullptr);
    REQUIRE(gauche->getToken().type == TOKEN_PLUS_GRAND);

    // Droite du && = '<'
    auto* droite = dynamic_cast<NoeudOperation*>(condition->getDroite());
    REQUIRE(droite != nullptr);
    REQUIRE(droite->getToken().type == TOKEN_PLUS_PETIT);
}

// Test while simple
TEST_CASE("Construction Arbre While simple", "[AST][Branch]")
{
    cout << "\nTest 7 - Arbre while simple" << endl;
    EnvironnementAST env;

    INoeud* arbre = construireArbreDepuisString(env,
        "while (i < 10) { aff i = i + 1; }"
    );

    REQUIRE(arbre != nullptr);

    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);
    REQUIRE(racine->getEnfants().size() == 1);

    // 1. NoeudWhile
    auto* noeudWhile = dynamic_cast<NoeudWhile*>(racine->getEnfants()[0]);
    REQUIRE(noeudWhile != nullptr);

    // 2. Condition '<'
    auto* condition = dynamic_cast<NoeudOperation*>(noeudWhile->getNoeudCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_PLUS_PETIT);

    // 3. Bloc while a 1 instruction
    auto* blocWhile = dynamic_cast<NoeudInstruction*>(noeudWhile->getNoeudBlocWhile());
    REQUIRE(blocWhile != nullptr);
    REQUIRE(blocWhile->getEnfants().size() == 1);

    // 4. Bloc fin while existe
    REQUIRE(noeudWhile->getNoeudBlocFinWhile() != nullptr);
}

// Test while avec condition logique ||
TEST_CASE("Construction Arbre While condition OU", "[AST][Branch]")
{
    cout << "\nTest 8 - Arbre while avec ||" << endl;
    EnvironnementAST env;

    INoeud* arbre = construireArbreDepuisString(env,
        "while (x == 0 || y == 0) { aff x = 1; }"
    );

    REQUIRE(arbre != nullptr);

    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);

    auto* noeudWhile = dynamic_cast<NoeudWhile*>(racine->getEnfants()[0]);
    REQUIRE(noeudWhile != nullptr);

    // Condition racine = ||
    auto* condition = dynamic_cast<NoeudOperation*>(noeudWhile->getNoeudCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_OU);

    // Gauche du || = '=='
    auto* gauche = dynamic_cast<NoeudOperation*>(condition->getGauche());
    REQUIRE(gauche != nullptr);
    REQUIRE(gauche->getToken().type == TOKEN_EGAL_EGAL);

    // Droite du || = '=='
    auto* droite = dynamic_cast<NoeudOperation*>(condition->getDroite());
    REQUIRE(droite != nullptr);
    REQUIRE(droite->getToken().type == TOKEN_EGAL_EGAL);
}

// Test while avec plusieurs instructions dans le corps
TEST_CASE("Construction Arbre While plusieurs instructions", "[AST][Branch]")
{
    cout << "\nTest 9 - Arbre while avec plusieurs instructions" << endl;
    EnvironnementAST env;

    INoeud* arbre = construireArbreDepuisString(env,
        "while (i < 5) { aff a = 0; aff i = i + 1; }"
    );

    REQUIRE(arbre != nullptr);

    auto* racine = dynamic_cast<NoeudInstruction*>(arbre);
    REQUIRE(racine != nullptr);

    auto* noeudWhile = dynamic_cast<NoeudWhile*>(racine->getEnfants()[0]);
    REQUIRE(noeudWhile != nullptr);

    // Bloc while contient 2 instructions (aff + aff)
    auto* blocWhile = dynamic_cast<NoeudInstruction*>(noeudWhile->getNoeudBlocWhile());
    REQUIRE(blocWhile != nullptr);
    REQUIRE(blocWhile->getEnfants().size() == 2);
}