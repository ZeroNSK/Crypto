#pragma once
#include <cstddef> // для size_t
extern "C" {
    void* encrypt(const unsigned char* data, size_t size, int a, int b, size_t* outSize);
    void* decrypt(const unsigned char* data, size_t size, int a, int b, size_t* outSize);
    void  free_buffer(void* ptr);
}
