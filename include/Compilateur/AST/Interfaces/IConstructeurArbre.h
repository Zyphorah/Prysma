#ifndef EEA73704_BBAE_43AD_9799_F1F919E04250
#define EEA73704_BBAE_43AD_9799_F1F919E04250

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "llvm/Support/Allocator.h"
#include <cstddef>
#include <new>
#include <utility>
#include <vector>

class IConstructeurArbre
{
public:
    IConstructeurArbre() = default;
    IConstructeurArbre(const IConstructeurArbre&) = delete;
    auto operator=(const IConstructeurArbre&) -> IConstructeurArbre& = delete;
    IConstructeurArbre(IConstructeurArbre&&) = delete;
    auto operator=(IConstructeurArbre&&) -> IConstructeurArbre& = delete;

    virtual ~IConstructeurArbre() = default;
    virtual auto construire(std::vector<Token>& tokens) -> INoeud* = 0;
    virtual auto construire(std::vector<Token>& tokens, int& index) -> INoeud* = 0;
    virtual auto getArena() -> llvm::BumpPtrAllocator& = 0;

    template<typename T, typename... Args>
    auto allouer(Args&&... args) -> T* {
        void* mem = getArena().Allocate(sizeof(T), alignof(T));
        return new (mem) T(std::forward<Args>(args)...); // NOLINT(cppcoreguidelines-owning-memory)
    }

    static constexpr std::size_t kArenaAlignment = 8;

    auto operator new(size_t taille) -> void* { return ::operator new(taille); }
    static void operator delete(void* ptr) { ::operator delete(ptr); }

    auto operator new(size_t taille, llvm::BumpPtrAllocator& arena) -> void* {
        return arena.Allocate(taille, kArenaAlignment); 
    }
    
    static void operator delete(void* ptr, llvm::BumpPtrAllocator& allocator) {
        // On ne fait rien ici ! 
        // Le BumpPtrAllocator libère toute la mémoire d'un coup à sa destruction.
        (void)ptr;
        (void)allocator;
    }
};

#endif /* EEA73704_BBAE_43AD_9799_F1F919E04250 */
