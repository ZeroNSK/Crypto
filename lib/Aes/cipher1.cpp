#include "cipher_interface.h"
#include "aes.h"
#include <vector>
#include <cstring>

using namespace std;

const size_t BLOCK_SIZE = 16;

// AES-ключ
uint8_t key[BLOCK_SIZE] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};

// IV
uint8_t iv[BLOCK_SIZE] = {
    0xA0, 0xA1, 0xA2, 0xA3,
    0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xAB,
    0xAC, 0xAD, 0xAE, 0xAF
};

// Общая CFB-функция
vector<unsigned char> AES_CFB(const vector<unsigned char>& input, bool encryptMode) {
    uint8_t roundKeys[176];
    AES128_KeyExpansion(key, roundKeys); // генерация раундовых ключей и вектора

    vector<unsigned char> result;
    vector<unsigned char> ivBuffer(iv, iv + BLOCK_SIZE);

    size_t totalBlocks = (input.size() + BLOCK_SIZE - 1) / BLOCK_SIZE; // обрабатывает данные поблочно

    for (size_t block = 0; block < totalBlocks; ++block) {
        uint8_t encryptedIV[BLOCK_SIZE]; 
        AES128_EncryptBlock(ivBuffer.data(), roundKeys, encryptedIV); // шифруется наш вектор

        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            size_t idx = block * BLOCK_SIZE + i;
            if (idx >= input.size()) break; 
            uint8_t inputByte = input[idx];
            uint8_t outByte = inputByte ^ encryptedIV[i];
            result.push_back(outByte); // байты с вектора xor'ятся с входными байтами

            if (encryptMode)
                ivBuffer[i] = outByte;
            else
                ivBuffer[i] = inputByte;
        }
    }

    return result;
}

// Экспортируемые функции для main
extern "C" {

void* encrypt(const unsigned char* data, size_t size, int, int, size_t* outSize) {
    vector<unsigned char> input(data, data + size);                                // копируются байты из одного в другое, шифруются
    vector<unsigned char> output = AES_CFB(input, true);                           // рез. записывается в uchar 
    *outSize = output.size();
    unsigned char* buffer = new unsigned char[output.size()];
    memcpy(buffer, output.data(), output.size());
    return buffer;
}

void* decrypt(const unsigned char* data, size_t size, int, int, size_t* outSize) {
    vector<unsigned char> input(data, data + size);
    vector<unsigned char> output = AES_CFB(input, false);
    *outSize = output.size();
    unsigned char* buffer = new unsigned char[output.size()];
    memcpy(buffer, output.data(), output.size());
    return buffer;
}

void free_buffer(void* ptr) {
    delete[] static_cast<unsigned char*>(ptr);
}

}
