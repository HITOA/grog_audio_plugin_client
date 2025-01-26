#include <grog_audio_plugin_client/allocator.hpp>

#include "tlsf.h"
#include <iostream>

#define GROG_ALLOCATOR_TLSF_BUFFER_SIZE 1e6


Grog::Allocator::Allocator(size_t size) {
    mem = malloc(size);
    tlsf = tlsf_create_with_pool(mem, size);
}

Grog::Allocator::~Allocator() {
    tlsf_destroy(tlsf);
    free(mem);
}

void* Grog::Allocator::Allocate(size_t size) {
    return tlsf_malloc(tlsf, size);
}

void* Grog::Allocator::AllocateAlign(size_t size, size_t align) {
    return tlsf_memalign(tlsf, align, size);
}

void* Grog::Allocator::Reallocate(void* ptr, size_t size) {
    return tlsf_realloc(tlsf, ptr, size);
}

void Grog::Allocator::Deallocate(void* ptr) {
    tlsf_free(tlsf, ptr);
}