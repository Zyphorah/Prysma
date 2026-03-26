#ifndef A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F
#define A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F

#include <string>
#include <stdexcept>

class ErrorHelper {
public:
    template<typename T>
    static auto verifyNotNull(T* ptr, const std::string& message) -> T* {
        if (ptr == nullptr) {
            throw std::runtime_error("Semantic error: " + message);
        }
        return ptr;
    }
    
    static void compilationError(const std::string& message);
    
    template<typename Registry, typename Key>
    static void verifyExistence(Registry& registry, const Key& key, const std::string& message) {
        if (!registry.exists(key)) {
            throw std::runtime_error("Semantic error: " + message);
        }
    }
};

#endif /* A31A7B4C_C1E7_42C6_AD7A_3C54664C1C1F */
