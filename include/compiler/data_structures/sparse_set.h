#include <vector>
#include <iostream>
#include <type_traits>
#include <cstddef>
#include <string>
#include "compiler/macros/prysma_noreturn.h"

// PROBABLEMENT REFAIRE DE ZÉRO TRÈS TRÈS PROCHAINEMENT 

// C'est du vieux code de Décembre 2025, il faudrait éventuellement le refaire.
// Il est évidemment bien trop complexe.

template<typename T>
struct is_index_type {
    static constexpr bool value =
        std::disjunction_v<
            std::is_same<T, std::size_t>,
            std::is_same<T, unsigned int>,
            std::is_same<T, unsigned long>,
            std::is_same<T, unsigned long long>
        >;
}; template<typename T>
inline constexpr bool is_index_type_v = is_index_type<T>::value;

template<typename T>
class sparse_set {
private:
    static constexpr std::size_t DEFAULT_DENSE_CAPACITY = 2048;
    static constexpr std::size_t DEFAULT_SPARSE_CAPACITY = 16384;

    using ContainedType = T;

    PRYSMA_NORETURN void error_not_enough_capacity(const std::string& context, std::size_t required, std::size_t actual);
    PRYSMA_NORETURN void error_entity_has_no_component(std::size_t entity_id);
    PRYSMA_NORETURN void error_entity_already_has_component(std::size_t entity_id);

    bool is_valid_entity_id(std::size_t entity_id) const noexcept; // n'est pas constexpr avant C++20

public:
    sparse_set(std::size_t init_dense_capacity = DEFAULT_DENSE_CAPACITY,
               std::size_t init_sparse_capacity = DEFAULT_SPARSE_CAPACITY);

    sparse_set(const sparse_set&) = default;
    sparse_set& operator=(const sparse_set&) = default;
    sparse_set(sparse_set&&) noexcept = default;
    sparse_set& operator=(sparse_set&&) noexcept = default;
    ~sparse_set() = default;

public:
    template<typename U>
    void insert(std::size_t entity_id, U&& component) noexcept;

    template<typename... Ts, typename U>
    auto batch_insert(Ts... entity_ids, const U& component) noexcept ->
        std::enable_if_t<(is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0), void>;

    auto emplace(std::size_t entity_id) noexcept ->
        std::enable_if_t<std::is_default_constructible_v<T>, void>;

    template<typename... Ts>
    auto batch_emplace(Ts... entity_ids) noexcept ->
        std::enable_if_t<std::is_default_constructible_v<T>
        && (is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0), void>;

    void remove_swap(std::size_t entity_id);

    template<typename... Ts>
    auto batch_remove_swap(Ts... entity_ids) noexcept ->
        std::enable_if_t<((is_index_type_v<Ts>) && ...)
        && (sizeof...(Ts) > 0), void>;

public:
    bool contains(std::size_t entity_id) const;

    template<typename... Ts>
    auto batch_contains(Ts... entity_ids) const noexcept ->
        std::enable_if_t<(is_index_type_v<Ts> && ...)
        && (sizeof...(Ts) > 0), bool>;

    std::size_t count() const noexcept;
    std::size_t capacity() const noexcept;
    std::size_t empty() const noexcept;

    void clear_sparse() noexcept;
    void clear_dense() noexcept;
    void clear_binding() noexcept;

    void reserve(std::size_t new_capacity) noexcept;

    void shrink_sparse_to_fit() noexcept;
    void shrink_dense_to_fit() noexcept;
    void shrink_binding_to_fit() noexcept;

public:
    auto begin() noexcept;
    auto end() noexcept;
    auto begin() const noexcept;
    auto end() const noexcept;

public:
    T* operator[](std::size_t entity_id) noexcept;
    const T* operator[](std::size_t entity_id) const noexcept;

    T* get(std::size_t entity_id) & noexcept;
    const T* get(std::size_t entity_id) const& noexcept;

private:
    std::vector<std::size_t> sparse_;
    std::vector<std::size_t> reverse_;
    std::vector<T> dense_;
};

//#include "sparse_set.tpp"