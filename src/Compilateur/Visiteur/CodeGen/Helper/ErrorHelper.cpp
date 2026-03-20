#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include <stdexcept>
#include <string>

void ErrorHelper::erreurCompilation(const std::string& message) {
    throw std::runtime_error("Erreur sémantique : " + message);
}
