#ifndef EEA73704_BBAE_43AD_9799_F1F919E04250
#define EEA73704_BBAE_43AD_9799_F1F919E04250

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include "llvm/Support/Allocator.h"
#include <cstddef>
#include <new>
#include <utility>
#include <vector>

class IBuilderTree
{
private:
    struct Finalizer {
        void (*destroy)(void*);
        void* pointeur;
    };
    std::vector<Finalizer> finalizers; // TODO : remplacer par une solutions llvm::StringRef vue car la destruction est temporaire actuellement, je veux utiliser la puissance du bump allocator
public:
    IBuilderTree() = default;
    IBuilderTree(const IBuilderTree&) = delete;
    auto operator=(const IBuilderTree&) -> IBuilderTree& = delete;
    IBuilderTree(IBuilderTree&&) = delete;
    auto operator=(IBuilderTree&&) -> IBuilderTree& = delete;

    virtual ~IBuilderTree() {
        for (auto it = finalizers.rbegin(); it != finalizers.rend(); ++it) {
            it->destroy(it->pointeur);
        }
    }
    
    virtual auto construire(std::vector<Token>& tokens) -> INode* = 0;
    virtual auto construire(std::vector<Token>& tokens, int& index) -> INode* = 0;
    virtual auto getArena() -> llvm::BumpPtrAllocator& = 0;

    template<typename T, typename... Args>
    auto allouer(Args&&... args) -> T* {
        void* mem = getArena().Allocate(sizeof(T), alignof(T));
        T* obj = new (mem) T(std::forward<Args>(args)...); // NOLINT(cppcoreguidelines-owning-memory)
        finalizers.push_back({[](void* ptr) { static_cast<T*>(ptr)->~T(); }, mem});
        return obj;
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
