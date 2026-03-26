#ifndef A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27
#define A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27

#include "Compiler/Parser/Equation/Interfaces/IManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class OperatorManager;

/**
 * @class ChainOfResponsibility
 * @brief Manages the chain of responsibility for operator detection
 * Determines operator precedence
 */
class ChainOfResponsibility {
private:
    IOperatorManager* _start;
    IManagerParenthesis* _parenthesisManager;
    std::vector<OperatorManager*> _operators;

public:
    /**
     * @brief Constructor
     * @param parenthesisManager Parenthesis management manager
     * @param operators Vector of operator managers in priority order
     */
    ChainOfResponsibility(
        IManagerParenthesis* parenthesisManager, 
        std::vector<OperatorManager*> operators
    );
    
    /**
     * @brief Destructor
     */
    ~ChainOfResponsibility();

    ChainOfResponsibility(const ChainOfResponsibility&) = delete;
    auto operator=(const ChainOfResponsibility&) -> ChainOfResponsibility& = delete;
    ChainOfResponsibility(ChainOfResponsibility&&) = delete;
    auto operator=(ChainOfResponsibility&&) -> ChainOfResponsibility& = delete;
    
    /**
     * @brief Finds the next operator to process according to priority
     * @param equation The equation to analyze
     * @return The index of the operator, or -1 if none found
     */
    [[nodiscard]] auto findOperator(const std::vector<Token>& equation) const -> int;
};

#endif /* A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27 */
