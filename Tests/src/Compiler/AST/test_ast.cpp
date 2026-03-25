#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <llvm-18/llvm/Support/Allocator.h>
#include <llvm/Support/TargetSelect.h>
#include "Compiler/Lexer/TokenType.h"
#include "catch.hpp"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/AST/BuilderTreeInstruction.h"
#include "Compiler/Builder/Equation/BuilderEquationFlottante.h"
#include "Compiler/AST/Registry/RegistryInstruction.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/AST_Genere.h"

// Expressions
#include "Compiler/Math/ExpressionLiteral.h"
#include "Compiler/Variable/ExpressionIdentifiant.h"
#include "Compiler/Variable/ExpressionRefVariable.h"
#include "Compiler/Variable/ExpressionUnRefVariable.h"
#include "Compiler/Math/ExpressionNegation.h"
#include "Compiler/Math/ExpressionString.h"
#include "Compiler/Array/ExpressionArrayInitialization.h"
#include "Compiler/Function/ExpressionCallFunction.h"

// Parsers d'instructions
#include "Compiler/Function/ParserDeclarationFunction.h"
#include "Compiler/Variable/ParserAssignmentVariable.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Variable/ParserDeclarationVariable.h"
#include "Compiler/Variable/ParserRefVariable.h"
#include "Compiler/Variable/ParserUnRefVariable.h"
#include "Compiler/Function/ParserReturn.h"
#include "Compiler/Function/ParserArgFunction.h"
#include "Compiler/Instruction/ParserIf.h"
#include "Compiler/Instruction/ParserWhile.h"
#include "Compiler/Instruction/ParserInclude.h"

using namespace std;




struct EnvironnementAST {
    llvm::BumpPtrAllocator arena;
    std::unique_ptr<RegistryInstruction> registryInstruction;
    std::unique_ptr<RegistryExpression> registryExpression;
    std::unique_ptr<RegistryType> registryType;
    std::unique_ptr<RegistryVariable> registryVariable;

    BuilderTreeInstruction* builderTree = nullptr;
    BuilderEquationFlottante* builderEquation = nullptr;
    ParserType* parserType = nullptr;
    ContextParser* contextParser = nullptr;
    ContextExpression* contexteExpression = nullptr;

    EnvironnementAST() {
    
        registryInstruction = std::make_unique<RegistryInstruction>();
        registryExpression = std::make_unique<RegistryExpression>();
        registryType = std::make_unique<RegistryType>();
        registryVariable = std::make_unique<RegistryVariable>();

        // Builder d'tree d'instruction 
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmismatched-new-delete"
        builderTree = new (arena) BuilderTreeInstruction(registryInstruction.get(), arena);
        #pragma GCC diagnostic pop

        //  Strategie d'équation 
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wmismatched-new-delete"
        builderEquation = new (arena) BuilderEquationFlottante(registryExpression.get(), arena);
        #pragma GCC diagnostic pop

        parserType = new (arena.Allocate<ParserType>()) ParserType(registryType.get(), builderEquation->recupererBuilderTree());

        // Créer le ContextParser
        contextParser = new (arena.Allocate<ContextParser>()) ContextParser(ContextParser::Dependencies{
            builderEquation->recupererBuilderTree(),
            builderTree,
            parserType,
            registryVariable.get(),
            registryType.get()
        });

        contexteExpression = new (arena.Allocate<ContextExpression>()) ContextExpression(
            builderEquation->recupererBuilderTree(),
            builderTree,
            parserType,
            contextParser,
            &arena,
            registryVariable.get(),
            registryType.get()
        );

        auto* exprLitInt = new (arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*contexteExpression);
        registryExpression->enregistryr(TOKEN_LIT_INT, exprLitInt);

        auto* exprLitFloat = new (arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*contexteExpression);
        registryExpression->enregistryr(TOKEN_LIT_FLOAT, exprLitFloat);

        auto* exprLitBool = new (arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*contexteExpression);
        registryExpression->enregistryr(TOKEN_LIT_BOLEEN, exprLitBool);

        auto* exprIdentifiant = new (arena.Allocate<ExpressionIdentifiant>()) ExpressionIdentifiant(*contexteExpression);
        registryExpression->enregistryr(TOKEN_IDENTIFIANT, exprIdentifiant);

        auto* exprRef = new (arena.Allocate<ExpressionRefVariable>()) ExpressionRefVariable(*contexteExpression);
        registryExpression->enregistryr(TOKEN_REF, exprRef);

        auto* exprUnRef = new (arena.Allocate<ExpressionUnRefVariable>()) ExpressionUnRefVariable(*contexteExpression);
        registryExpression->enregistryr(TOKEN_UNREF, exprUnRef);

        auto* exprNeg = new (arena.Allocate<ExpressionNegation>()) ExpressionNegation(*contexteExpression);
        registryExpression->enregistryr(TOKEN_NON, exprNeg);

        auto* exprString = new (arena.Allocate<ExpressionString>()) ExpressionString(*contexteExpression);
        registryExpression->enregistryr(TOKEN_GUILLEMET, exprString);

        auto* exprTab = new (arena.Allocate<ExpressionArrayInitialization>()) ExpressionArrayInitialization(*contexteExpression);
        registryExpression->enregistryr(TOKEN_CROCHET_OUVERT, exprTab);

        auto* exprCall = new (arena.Allocate<ExpressionCallFunction>()) ExpressionCallFunction(*contexteExpression);
        registryExpression->enregistryr(TOKEN_CALL, exprCall);

        // Parsers d'instructions
        auto* parsFonc = new (arena.Allocate<ParserDeclarationFunction>()) ParserDeclarationFunction(*contextParser);
        registryInstruction->enregistryr(TOKEN_FONCTION, parsFonc);

        auto* parsAff = new (arena.Allocate<ParserAssignmentVariable>()) ParserAssignmentVariable(*contextParser);
        registryInstruction->enregistryr(TOKEN_AFF, parsAff);

        auto* parsDec = new (arena.Allocate<ParserDeclarationVariable>()) ParserDeclarationVariable(*contextParser);
        registryInstruction->enregistryr(TOKEN_DEC, parsDec);

        auto* parsCall = new (arena.Allocate<ParserCallFunction>()) ParserCallFunction(*contextParser);
        registryInstruction->enregistryr(TOKEN_CALL, parsCall);

        auto* parsRet = new (arena.Allocate<ParserReturn>()) ParserReturn(*contextParser);
        registryInstruction->enregistryr(TOKEN_RETOUR, parsRet);

        auto* parsArg = new (arena.Allocate<ParserArgFunction>()) ParserArgFunction(*contextParser);
        registryInstruction->enregistryr(TOKEN_ARG, parsArg);

        auto* parsUnRef = new (arena.Allocate<ParserUnRefVariable>()) ParserUnRefVariable(*contextParser);
        registryInstruction->enregistryr(TOKEN_UNREF, parsUnRef);

        auto* parsRefVar = new (arena.Allocate<ParserRefVariable>()) ParserRefVariable(*contextParser);
        registryInstruction->enregistryr(TOKEN_REF, parsRefVar);

        auto* parsIf = new (arena.Allocate<ParserIf>()) ParserIf(*contextParser);
        registryInstruction->enregistryr(TOKEN_SI, parsIf);

        auto* parsWhile = new (arena.Allocate<ParserWhile>()) ParserWhile(*contextParser);
        registryInstruction->enregistryr(TOKEN_TANT_QUE, parsWhile);

        auto* parsInclude = new (arena.Allocate<ParserInclude>()) ParserInclude(*contextParser);
        registryInstruction->enregistryr(TOKEN_INCLUDE, parsInclude);
    }
};


/// Construit un tree d'équation à partir d'une string de code source.
INode* construireEquationDepuisString(EnvironnementAST& env, const std::string& code) {
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenizer(code);

    // Retirer le token EOF ajouté par le Lexer
    if (!tokens.empty() && tokens.back().type == TOKEN_EOF) {
        tokens.pop_back();
    }

    return env.builderEquation->construire(tokens);
}

/// Construit un tree d'instructions à partir d'une string de code source.
INode* construireTreeDepuisString(EnvironnementAST& env, const std::string& code) {
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenizer(code);
    return env.builderTree->construire(tokens);
}

template<typename TypeAttendu, typename TypeInputr>

TypeAttendu* verifierTypeEtCaster(TypeInputr* node) {
    auto* nodeCast = dynamic_cast<TypeAttendu*>(node);
    REQUIRE(nodeCast != nullptr);
    return nodeCast;
}


template<auto getter, typename TypeAttendu, typename TypeValeur, auto accessor = nullptr>
struct matcherNode
{
    TypeValeur valeurAttendue;
    
    matcherNode(TypeValeur p_valeurAttendue) : valeurAttendue(std::move(p_valeurAttendue)) {}

    template<typename typeBase>
    void operator()(typeBase* node) {
       auto* nodeCast = verifierTypeEtCaster<TypeAttendu>(node);
       if constexpr (accessor == nullptr) {
           REQUIRE((nodeCast->*getter)() == valeurAttendue);
       } else {
           REQUIRE(((nodeCast->*getter)().*accessor) == valeurAttendue);
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

    matcherGeneralBinaire(const TypeValeur& p_valeur, Gauche p_nodeGaucheAttendu, Droite p_nodeDroiteAttendu) 
        : verificateurGauche(std::move(p_nodeGaucheAttendu)), verificateurDroite(std::move(p_nodeDroiteAttendu)), valeurAttendue(p_valeur) {}

    template<typename typeBase>
    void operator()(typeBase* node) {
        auto* nodeOperation = verifierTypeEtCaster<TypeCible>(node);
        
        if constexpr (accessor == nullptr) {
            REQUIRE((nodeOperation->*getter)() == valeurAttendue);
        } else {
            REQUIRE(((nodeOperation->*getter)().*accessor) == valeurAttendue);
        }
        verificateurDroite(nodeOperation->getDroite());
        verificateurGauche(nodeOperation->getGauche());
    }
};

// Matcher pour une liste d'child d'un node instruction ou autre
template<typename TypeCible, auto getter, typename... Matchers>
struct matcherListsChild 
{
    std::tuple<Matchers...> verificateurs;

    matcherListsChild(Matchers... p_verificateurs) : verificateurs(std::make_tuple(std::move(p_verificateurs)...)) {}

    template<typename typeBase>
    void operator()(typeBase* node) {
        auto* nodeCast = verifierTypeEtCaster<TypeCible>(node);

        // Je ne dois pas faire que get child car il peux y avoir nodeIf nodeWhile
        const auto& childs = (nodeCast->*getter)();
        REQUIRE(childs.size() == sizeof...(Matchers));
        verifierChilds(childs, std::index_sequence_for<Matchers...>{});
    }
};


// Faire les helper pour éviter d'avoir beaucoup de syntaxe dans les tests template 
auto Literal(const std::string& valeur) {
    return matcherNode<&NodeLiteral::getToken, NodeLiteral, std::string, &Token::value>(valeur);
}

// Helper pour les opérations
auto operateur() {
    return [](const std::string& type, auto gauche, auto droite) {
        return matcherGeneralBinaire<NodeOperation, &NodeOperation::getToken, std::string, &Token::value, decltype(gauche), decltype(droite)>(
            type, gauche, droite
        );
    };
}

// Helper pour les listes d'child
template<typename T, auto Method, typename M>
struct matcherPropriete {
    M m;
    matcherPropriete(M m) : m(std::move(m)) {}
    void operator()(INode* n) {
        auto* t = verifierTypeEtCaster<T>(n);
        m((t->*Method)()); 
    }
};

// matcher utilitaire pour appliquer plusieurs matcher sur le même node
template<typename T, typename... Ms>
struct matcherCombine {
    std::tuple<Ms...> ms;
    matcherCombine(Ms... ms) : ms(std::make_tuple(std::move(ms)...)) {}
    void operator()(INode* n) {
        auto* t = verifierTypeEtCaster<T>(n);
        // Applique chaque matcher au nœud casté
        std::apply([&](auto&&... args) { (args(t), ...); }, ms);
    }
};

// C'est un fichier de test exclusif à l'tree syntaxique abstrait première partie du test sera de vérifier la construction de l'tree 
// d'equation et de tester des operations de base comme l'addition, la soustraction, la multiplication et la division.

TEST_CASE("Construction Tree Equation Simple", "[AST]")
{
    cout<< "\nTest 1 - Construction d'un tree pour l'equation '1 + 10 * 50'" << endl;
    
    EnvironnementAST env;
    INode* tree = construireEquationDepuisString(env, "1 + 10 * 50");

    auto verificateur = 
    operateur()(
        "+", 
        Literal("1"), 
        operateur()(
            "*", 
            Literal("10"), 
            Literal("50")
        ));
    
    verificateur(tree);
}

TEST_CASE("Builder Tree equation priorite", "[AST]")
{
    cout << "\nTest 2 - Construction d'un tree avec les regles Prysma" << endl;
    EnvironnementAST env;
    INode* tree = construireEquationDepuisString(env, "40 / 2 + 10 - 5 * 3");
    
    auto verificateur = 
    operateur()(
        "+", 
        operateur()(
            "/", 
            Literal("40"), 
            Literal("2")
        ), 
        operateur()(
            "-", 
            Literal("10"), 
            operateur()(
                "*", 
                Literal("5"), 
                Literal("3")
            )
        )
    );
    verificateur(tree);
}

TEST_CASE("Builder Tree equation depth parenthèse", "[AST]")
{
    cout << "\nTest 3 - Construction d'un tree avec des parenthèses" << endl;
    EnvironnementAST env;
    INode* tree = construireEquationDepuisString(env, "(((40/2 +10)+ 5 * 3)+10)");

    auto verificateur = 
    operateur()(
        "+", 
        operateur()(
            "+", 
            operateur()(
                "+", 
                operateur()(
                    "/", 
                    Literal("40"), 
                    Literal("2")
                ), 
                Literal("10")
            ), 
            operateur()(
                "*", 
                Literal("5"), 
                Literal("3")
            )
        ), 
        Literal("10")
    );

    verificateur(tree);
}

// Début des tests pour l'tree d'instruction 

// Test de branchement if simple avec else
TEST_CASE("Construction Tree If simple avec else", "[AST][Branch]")
{
    cout << "\nTest 4 - Tree if/else simple" << endl;
    EnvironnementAST env;

    // Code Prysma : if avec condition et deux branches
    INode* tree = construireTreeDepuisString(env,
        "if (a > 5) { aff x = 1; } else { aff x = 2; }"
    );
     
    REQUIRE(tree != nullptr);

    // 1. Racine = NodeInstruction qui contient le if
    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);
    REQUIRE(racine->getChilds().size() == 1);

    // 2. Premier child = NodeIf
    auto* nodeIf = dynamic_cast<NodeIf*>(racine->getChilds()[0]);
    REQUIRE(nodeIf != nullptr);

    // 3. Condition pas nulle
    REQUIRE(nodeIf->getNodeCondition() != nullptr);

    // 4. Vérifier condition : opération '>'
    auto* condition = dynamic_cast<NodeOperation*>(nodeIf->getNodeCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_PLUS_GRAND);

    // 5. Bloc if existe et contient 1 instruction
    auto* blocIf = dynamic_cast<NodeInstruction*>(nodeIf->getNodeBlocIf());
    REQUIRE(blocIf != nullptr);
    REQUIRE(blocIf->getChilds().size() == 1);

    // 6. Bloc else existe et contient 1 instruction
    auto* blocElse = dynamic_cast<NodeInstruction*>(nodeIf->getNodeBlocElse());
    REQUIRE(blocElse != nullptr);
    REQUIRE(blocElse->getChilds().size() == 1);

    // 7. Bloc endif existe (node de output)
    REQUIRE(nodeIf->getNodeBlocEndif() != nullptr);
}

// Test if sans else
TEST_CASE("Construction Tree If sans else", "[AST][Branch]")
{
    cout << "\nTest 5 - Tree if sans else" << endl;
    EnvironnementAST env;

    INode* tree = construireTreeDepuisString(env,
        "if (a == 10) { aff y = 42; }"
    );

    REQUIRE(tree != nullptr);

    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);

    // 1. NodeIf
    auto* nodeIf = dynamic_cast<NodeIf*>(racine->getChilds()[0]);
    REQUIRE(nodeIf != nullptr);

    // 2. Condition == 
    auto* condition = dynamic_cast<NodeOperation*>(nodeIf->getNodeCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_EGAL_EGAL);

    // 3. Bloc if a 1 child
    auto* blocIf = dynamic_cast<NodeInstruction*>(nodeIf->getNodeBlocIf());
    REQUIRE(blocIf != nullptr);
    REQUIRE(blocIf->getChilds().size() == 1);

    // 4. Pas de else
    REQUIRE(nodeIf->getNodeBlocElse() == nullptr);

    // 5. Endif existe quand même
    REQUIRE(nodeIf->getNodeBlocEndif() != nullptr);
}

// Test if avec condition logique && 
TEST_CASE("Construction Tree If condition logique ET", "[AST][Branch]")
{
    cout << "\nTest 6 - Tree if avec &&" << endl;
    EnvironnementAST env;

    INode* tree = construireTreeDepuisString(env,
        "if (a > 1 && b < 10) { aff z = 0; }"
    );

    REQUIRE(tree != nullptr);

    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);

    auto* nodeIf = dynamic_cast<NodeIf*>(racine->getChilds()[0]);
    REQUIRE(nodeIf != nullptr);

    // Condition racine = &&
    auto* condition = dynamic_cast<NodeOperation*>(nodeIf->getNodeCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_ET);

    // Gauche du && = '>'
    auto* gauche = dynamic_cast<NodeOperation*>(condition->getGauche());
    REQUIRE(gauche != nullptr);
    REQUIRE(gauche->getToken().type == TOKEN_PLUS_GRAND);

    // Droite du && = '<'
    auto* droite = dynamic_cast<NodeOperation*>(condition->getDroite());
    REQUIRE(droite != nullptr);
    REQUIRE(droite->getToken().type == TOKEN_PLUS_PETIT);
}

// Test while simple
TEST_CASE("Construction Tree While simple", "[AST][Branch]")
{
    cout << "\nTest 7 - Tree while simple" << endl;
    EnvironnementAST env;

    INode* tree = construireTreeDepuisString(env,
        "while (i < 10) { aff i = i + 1; }"
    );

    REQUIRE(tree != nullptr);

    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);
    REQUIRE(racine->getChilds().size() == 1);

    // 1. NodeWhile
    auto* nodeWhile = dynamic_cast<NodeWhile*>(racine->getChilds()[0]);
    REQUIRE(nodeWhile != nullptr);

    // 2. Condition '<'
    auto* condition = dynamic_cast<NodeOperation*>(nodeWhile->getNodeCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_PLUS_PETIT);

    // 3. Bloc while a 1 instruction
    auto* blocWhile = dynamic_cast<NodeInstruction*>(nodeWhile->getNodeBlocWhile());
    REQUIRE(blocWhile != nullptr);
    REQUIRE(blocWhile->getChilds().size() == 1);

    // 4. Bloc fin while existe
    REQUIRE(nodeWhile->getNodeBlocFinWhile() != nullptr);
}

// Test while avec condition logique ||
TEST_CASE("Construction Tree While condition OU", "[AST][Branch]")
{
    cout << "\nTest 8 - Tree while avec ||" << endl;
    EnvironnementAST env;

    INode* tree = construireTreeDepuisString(env,
        "while (x == 0 || y == 0) { aff x = 1; }"
    );

    REQUIRE(tree != nullptr);

    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);

    auto* nodeWhile = dynamic_cast<NodeWhile*>(racine->getChilds()[0]);
    REQUIRE(nodeWhile != nullptr);

    // Condition racine = ||
    auto* condition = dynamic_cast<NodeOperation*>(nodeWhile->getNodeCondition());
    REQUIRE(condition != nullptr);
    REQUIRE(condition->getToken().type == TOKEN_OU);

    // Gauche du || = '=='
    auto* gauche = dynamic_cast<NodeOperation*>(condition->getGauche());
    REQUIRE(gauche != nullptr);
    REQUIRE(gauche->getToken().type == TOKEN_EGAL_EGAL);

    // Droite du || = '=='
    auto* droite = dynamic_cast<NodeOperation*>(condition->getDroite());
    REQUIRE(droite != nullptr);
    REQUIRE(droite->getToken().type == TOKEN_EGAL_EGAL);
}

// Test while avec plusieurs instructions dans le body
TEST_CASE("Construction Tree While plusieurs instructions", "[AST][Branch]")
{
    cout << "\nTest 9 - Tree while avec plusieurs instructions" << endl;
    EnvironnementAST env;

    INode* tree = construireTreeDepuisString(env,
        "while (i < 5) { aff a = 0; aff i = i + 1; }"
    );

    REQUIRE(tree != nullptr);

    auto* racine = dynamic_cast<NodeInstruction*>(tree);
    REQUIRE(racine != nullptr);

    auto* nodeWhile = dynamic_cast<NodeWhile*>(racine->getChilds()[0]);
    REQUIRE(nodeWhile != nullptr);

    // Bloc while contient 2 instructions (aff + aff)
    auto* blocWhile = dynamic_cast<NodeInstruction*>(nodeWhile->getNodeBlocWhile());
    REQUIRE(blocWhile != nullptr);
    REQUIRE(blocWhile->getChilds().size() == 2);
}