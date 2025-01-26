#pragma once

#include <cstdlib>



void* operator new(size_t size);
void operator delete(void* ptr) noexcept;

namespace Grog {

    class Allocator {
    public:
        Allocator() = delete;
        Allocator(size_t size);
        ~Allocator();

        void* Allocate(size_t size);
        void* AllocateAlign(size_t size, size_t align);
        void* Reallocate(void* ptr, size_t size);
        void Deallocate(void* ptr);

        template<typename T, typename... Args>
        T* Create(Args... args) {
            return new(Allocate(sizeof(T))) T(args...);
        }

        template<typename T>
        void Destroy(T* ptr) {
            ptr->~T();
            Deallocate(ptr);
        }

    private:
        void* mem = nullptr;
        void* tlsf = nullptr;
    };

};