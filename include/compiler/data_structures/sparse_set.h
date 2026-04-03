#include <cstdint>
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
};

template<typename T>
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

template<typename T>
PRYSMA_NORETURN void sparse_set<T>::error_not_enough_capacity(const std::string& context, std::size_t required, std::size_t actual) {
    std::cerr << "[ERROR] " << context
        << " | Required size: " << required
        << ", Actual size: " << actual << std::endl;
}

template<typename T>
PRYSMA_NORETURN void sparse_set<T>::error_entity_has_no_component(std::size_t entity_id) {
    std::cerr << "[ERROR] Entity " << entity_id
        << " does not have a component!" << std::endl;
}

template<typename T>
PRYSMA_NORETURN void sparse_set<T>::error_entity_already_has_component(std::size_t entity_id) {
    std::cerr << "[ERROR] Entity " << entity_id
        << " already has a component!" << std::endl;
}

template<typename T>
bool sparse_set<T>::is_valid_entity_id(std::size_t entity_id) const noexcept {
    return entity_id < sparse_.size();
}

template<typename T>
sparse_set<T>::sparse_set(std::size_t init_dense_capacity, std::size_t init_sparse_capacity) {
    dense_.reserve(init_dense_capacity);
    reverse_.reserve(init_dense_capacity);
    sparse_.reserve(init_sparse_capacity);
    sparse_.resize(init_sparse_capacity, SIZE_MAX);
}

template<typename T>
template<typename U>
void sparse_set<T>::insert(std::size_t entity_id, U&& component) noexcept {
    if (!is_valid_entity_id(entity_id)) {
        error_not_enough_capacity("Sparse vector too small", entity_id + 1, sparse_.size());
        return;
    }

    if (contains(entity_id)) {
        error_entity_already_has_component(entity_id);
        return;
    }

    dense_.push_back(std::forward<U>(component));
    std::size_t component_index = dense_.size() - 1;

    sparse_[entity_id] = component_index;
    reverse_.push_back(entity_id);
}

template<typename T>
template<typename... Ts, typename U>
auto sparse_set<T>::batch_insert(Ts... entity_ids, const U& component) noexcept
-> std::enable_if_t<(is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0), void> {
    (insert(entity_ids, component), ...);
}

template<typename T>
auto sparse_set<T>::emplace(std::size_t entity_id) noexcept
-> std::enable_if_t<std::is_default_constructible_v<T>, void> {
    if (!is_valid_entity_id(entity_id)) {
        error_not_enough_capacity("Sparse vector too small", entity_id + 1, sparse_.size());
        return;
    }

    std::cout << "Emplacing new component for entity " << entity_id << std::endl;

    if (contains(entity_id)) {
        error_entity_already_has_component(entity_id);
        return;
    }

    dense_.emplace_back();
    std::size_t component_index = dense_.size() - 1;

    sparse_[entity_id] = component_index;
    reverse_.push_back(entity_id);
}

template<typename T>
template<typename... Ts>
auto sparse_set<T>::batch_emplace(Ts... entity_ids) noexcept
-> std::enable_if_t<std::is_default_constructible_v<T>
&& (is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0), void> {
    (emplace(entity_ids), ...);
}

template<typename T>
void sparse_set<T>::remove_swap(std::size_t entity_id) {
    if (!is_valid_entity_id(entity_id)) {
        error_not_enough_capacity("Sparse vector too small", entity_id + 1, sparse_.size());
        return;
    }

    if (!contains(entity_id)) {
        error_entity_has_no_component(entity_id);
        return;
    }

    std::size_t component_index = sparse_[entity_id];
    std::size_t last_component_index = dense_.size() - 1;
    std::size_t last_entity_id = reverse_[last_component_index];

    std::swap(dense_[component_index], dense_[dense_.size() - 1]);
    dense_.pop_back();

    std::swap(reverse_[component_index], reverse_[last_component_index]);
    reverse_.pop_back();

    sparse_[last_entity_id] = component_index;
    sparse_[entity_id] = SIZE_MAX;
}

template<typename T>
template<typename... Ts>
auto sparse_set<T>::batch_remove_swap(Ts... entity_ids) noexcept
-> std::enable_if_t<((is_index_type_v<Ts>) && ...) && (sizeof...(Ts) > 0), void> {
    (remove_swap(entity_ids), ...);
}

template<typename T>
bool sparse_set<T>::contains(std::size_t entity_id) const {
    return is_valid_entity_id(entity_id) && sparse_[entity_id] != SIZE_MAX;
}

template<typename T>
template<typename... Ts>
auto sparse_set<T>::batch_contains(Ts... entity_ids) const noexcept
-> std::enable_if_t<(is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0), bool> {
    return (contains(entity_ids) && ...);
}

template<typename T>
std::size_t sparse_set<T>::count() const noexcept { return dense_.size(); }

template<typename T>
std::size_t sparse_set<T>::capacity() const noexcept { return sparse_.capacity(); }

template<typename T>
std::size_t sparse_set<T>::empty() const noexcept { return sparse_.empty(); }

template<typename T>
void sparse_set<T>::clear_sparse() noexcept { sparse_.clear(); }

template<typename T>
void sparse_set<T>::clear_dense() noexcept { dense_.clear(); }

template<typename T>
void sparse_set<T>::clear_binding() noexcept { reverse_.clear(); }

template<typename T>
void sparse_set<T>::reserve(std::size_t new_capacity) noexcept { sparse_.reserve(new_capacity); }

template<typename T>
void sparse_set<T>::shrink_sparse_to_fit() noexcept { sparse_.shrink_to_fit(); }

template<typename T>
void sparse_set<T>::shrink_dense_to_fit() noexcept { dense_.shrink_to_fit(); }

template<typename T>
void sparse_set<T>::shrink_binding_to_fit() noexcept { reverse_.shrink_to_fit(); }

template<typename T>
auto sparse_set<T>::begin() noexcept { return dense_.begin(); }

template<typename T>
auto sparse_set<T>::end() noexcept { return dense_.end(); }

template<typename T>
auto sparse_set<T>::begin() const noexcept { return dense_.begin(); }

template<typename T>
auto sparse_set<T>::end() const noexcept { return dense_.end(); }

template<typename T>
T* sparse_set<T>::operator[](std::size_t entity_id) noexcept {
    if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
        error_not_enough_capacity("Sparse vector too small or entity has no component", entity_id + 1, sparse_.size());
        return nullptr;
    }
    return &dense_[sparse_[entity_id]];
}

template<typename T>
const T* sparse_set<T>::operator[](std::size_t entity_id) const noexcept {
    if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
        error_not_enough_capacity("Sparse vector too small or entity has no component", entity_id + 1, sparse_.size());
        return nullptr;
    }
    return &dense_[sparse_[entity_id]];
}

template<typename T>
T* sparse_set<T>::get(std::size_t entity_id) & noexcept {
    if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
        error_not_enough_capacity("Sparse vector too small or entity has no component", entity_id + 1, sparse_.size());
        return nullptr;
    }
    return &dense_[sparse_[entity_id]];
}

template<typename T>
const T* sparse_set<T>::get(std::size_t entity_id) const& noexcept {
    if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
        error_not_enough_capacity("Sparse vector too small or entity has no component", entity_id + 1, sparse_.size());
        return nullptr;
    }
    return &dense_[sparse_[entity_id]];
}