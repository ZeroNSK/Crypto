#include "cipher_interface.h"
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int MODULO = 256;
const string keyFile = "last_vigenere_key.bin";

// Повтор ключа по длине данных
vector<unsigned char> repeatKey(const vector<unsigned char>& key, size_t length) {
    vector<unsigned char> fullKey(length);
    for (size_t i = 0; i < length; ++i)
        fullKey[i] = key[i % key.size()];
    return fullKey;
}

vector<unsigned char> vigenereEncrypt(const vector<unsigned char>& data, const vector<unsigned char>& key) {
    vector<unsigned char> output(data.size());
    for (size_t i = 0; i < data.size(); ++i)
        output[i] = (data[i] + key[i]) % MODULO;
    return output;
}

vector<unsigned char> vigenereDecrypt(const vector<unsigned char>& data, const vector<unsigned char>& key) {
    vector<unsigned char> output(data.size());
    for (size_t i = 0; i < data.size(); ++i)
        output[i] = (data[i] - key[i] + MODULO) % MODULO;
    return output;
}

// Генерация или загрузка ключа
vector<unsigned char> prepareKey(bool autoGen, int seed, size_t size, bool isEncrypt) {
    vector<unsigned char> baseKey;

    if (autoGen) {
        if (isEncrypt) {
            srand(static_cast<unsigned>(time(nullptr)));
            for (int i = 0; i < 16; ++i)
                baseKey.push_back(rand() % 256);

            ofstream out(keyFile, ios::binary);
            out.write(reinterpret_cast<const char*>(baseKey.data()), baseKey.size());
        } else {
            ifstream in(keyFile, ios::binary);
            if (!in) throw runtime_error("❌ Не удалось загрузить ключ из файла.");
            baseKey = vector<unsigned char>((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        }
    } else {
        srand(seed);
        for (int i = 0; i < 16; ++i)
            baseKey.push_back(rand() % 256);
    }

    return repeatKey(baseKey, size);
}

extern "C" {

void* encrypt(const unsigned char* data, size_t size, int autoGen, int keySeed, size_t* outSize) {
    vector<unsigned char> input(data, data + size);
    vector<unsigned char> fullKey = prepareKey(autoGen, keySeed, size, true);
    vector<unsigned char> result = vigenereEncrypt(input, fullKey);

    *outSize = result.size();
    unsigned char* buffer = new unsigned char[*outSize];
    memcpy(buffer, result.data(), *outSize);
    return buffer;
}

void* decrypt(const unsigned char* data, size_t size, int autoGen, int keySeed, size_t* outSize) {
    vector<unsigned char> input(data, data + size);
    vector<unsigned char> fullKey = prepareKey(autoGen, keySeed, size, false);
    vector<unsigned char> result = vigenereDecrypt(input, fullKey);

    *outSize = result.size();
    unsigned char* buffer = new unsigned char[*outSize];
    memcpy(buffer, result.data(), *outSize);
    return buffer;
}

void free_buffer(void* ptr) {
    delete[] static_cast<unsigned char*>(ptr);
}

}
