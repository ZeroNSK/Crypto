#include "../cipher_interface.h"
#include <vector>
#include <cstring>

using namespace std;


vector<unsigned char> encrypt_block(const vector<unsigned char>& data, int rows, int cols) {
    int blockSize = rows * cols; // размер одного блока
    vector<unsigned char> padded = data;
    while (padded.size() % blockSize != 0) // если не хватает то заполняет остатки нулями
        padded.push_back(0);

    vector<unsigned char> result; 
    for (size_t i = 0; i < padded.size(); i += blockSize) 
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col)
                result.push_back(padded[i + col * rows + row]); // переставляет байты по строкам

    return result; // итоговый зашифрованный вектор байтов
}

vector<unsigned char> decrypt_block(const vector<unsigned char>& data, int rows, int cols) {
    int blockSize = rows * cols; // все так же определяет размер блока
    vector<unsigned char> result;
    for (size_t i = 0; i < data.size(); i += blockSize)
        for (int col = 0; col < cols; ++col)
            for (int row = 0; row < rows; ++row) 
                result.push_back(data[i + row * cols + col]); // обратная операция для encrypt
    return result;
}

extern "C" {

void* encrypt(const unsigned char* data, size_t size, int rows, int cols, size_t* outSize) {
    vector<unsigned char> in(data, data + size); // копирует data в наш vector
    vector<unsigned char> out = encrypt_block(in, rows, cols); // вызывает шифровку
    *outSize = out.size(); 
    unsigned char* buf = new unsigned char[out.size()]; // записывает выход 
    memcpy(buf, out.data(), out.size()); // копирует size байт из data и записывает в buf 
    return buf;
}

void* decrypt(const unsigned char* data, size_t size, int rows, int cols, size_t* outSize) {
    vector<unsigned char> in(data, data + size);
    vector<unsigned char> out = decrypt_block(in, rows, cols);
    *outSize = out.size();
    unsigned char* buf = new unsigned char[out.size()];
    memcpy(buf, out.data(), out.size());
    return buf;
}

void free_buffer(void* ptr) {
    delete[] static_cast<unsigned char*>(ptr); //обычное освобождение памяти
}

}
