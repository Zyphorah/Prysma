#ifndef A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F
#define A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F

#include <string>
#include <stdexcept>

class ErrorHelper {
public:
    template<typename T>
    static T* verifierNonNull(T* ptr, const std::string& message) {
        if (ptr == nullptr) {
            throw std::runtime_error("Erreur sémantique : " + message);
        }
        return ptr;
    }
    
    static void erreurCompilation(const std::string& message);
    
    template<typename Registre, typename Cle>
    static void verifierExistence(Registre& registre, const Cle& cle, const std::string& message) {
        if (!registre.existe(cle)) {
            throw std::runtime_error("Erreur sémantique : " + message);
        }
    }
};

#endif /* A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F */
