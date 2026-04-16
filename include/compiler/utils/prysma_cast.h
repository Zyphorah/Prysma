#ifndef F4234AA8_0351_4542_BA1B_C14DFB55FAC4
#define F4234AA8_0351_4542_BA1B_C14DFB55FAC4

#include "compiler/macros/prysma_nodiscard.h"

namespace prysma {

// Checks if the value is of the target type
template <typename To, typename From>
PRYSMA_NODISCARD inline auto isa(const From* val) -> bool {
    if (!val) {
        return false;
    }
    return To::classof(val); // pourra changer lors de la transition vers le crtp en faveur d'un approche évaluée a la compilation
}

// Casts the value to the target type (static_cast)
template <typename To, typename From>
PRYSMA_NODISCARD inline auto cast(From* val) -> To* {
    return static_cast<To*>(val); 
}

// Dynamically casts the value to the target type, returns nullptr if not compatible
template <typename To, typename From>
PRYSMA_NODISCARD inline auto dyn_cast(From* val) -> To* {
    if (isa<To>(val)) {
        return cast<To>(val);
    }
    return nullptr;
}

} 

#endif /* F4234AA8_0351_4542_BA1B_C14DFB55FAC4 */
