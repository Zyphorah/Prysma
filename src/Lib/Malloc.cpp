#include <cstdlib>

extern "C" {
    auto prysma_malloc(size_t size) -> void* {
        return std::malloc(size); 
    }

    void prysma_free(void* ptr) {
        if (ptr != nullptr) { 
            std::free(ptr);
        }
    }
}
