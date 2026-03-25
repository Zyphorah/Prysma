#ifndef A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F
#define A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F

#include <string>
#include <stdexcept>

class ErrorHelper {
public:
    template<typename T>
    static auto verifierNonNull(T* ptr, const std::string& message) -> T* {
        if (ptr == nullptr) {
            throw std::runtime_error("Error sémantique : " + message);
        }
        return ptr;
    }
    
    static void errorCompilation(const std::string& message);
    
    template<typename Registry, typename Cle>
    static void verifierExistence(Registry& registry, const Cle& cle, const std::string& message) {
        if (!registry.existe(cle)) {
            throw std::runtime_error("Error sémantique : " + message);
        }
    }
};

#endif /* A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F */
