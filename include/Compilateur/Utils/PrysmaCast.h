#ifndef F4234AA8_0351_4542_BA1B_C14DFB55FAC4
#define F4234AA8_0351_4542_BA1B_C14DFB55FAC4
namespace prysma {

template <typename To, typename From>
[[nodiscard]] inline auto isa(const From* val) -> bool {
    if (!val) {
        return false;
    }
    return To::classof(val);
}

template <typename To, typename From>
[[nodiscard]] inline auto cast(From* val) -> To* {
    return static_cast<To*>(val); 
}

template <typename To, typename From>
[[nodiscard]] inline auto dyn_cast(From* val) -> To* {
    if (isa<To>(val)) {
        return cast<To>(val);
    }
    return nullptr;
}

} 

#endif /* F4234AA8_0351_4542_BA1B_C14DFB55FAC4 */
