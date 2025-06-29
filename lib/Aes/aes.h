#pragma once
#include <stdint.h> // ✅ чтобы знать что такое uint8_t

#ifdef __cplusplus
extern "C" {
#endif

void AES128_KeyExpansion(const uint8_t* key, uint8_t* roundKeys);
void AES128_EncryptBlock(const uint8_t* input, const uint8_t* roundKeys, uint8_t* output);
void AES128_DecryptBlock(const uint8_t* input, const uint8_t* roundKeys, uint8_t* output);

#ifdef __cplusplus
}
#endif
