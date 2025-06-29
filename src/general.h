#pragma once
#include <dlfcn.h>
#include <stdexcept>
#include <string>

// Все шифры проекта
enum CryptoAlgo {
    TABLE = 1,
    AES = 2,
    VIGENER = 3
};

// Функция загрузки библиотеки по алгоритму
void* loadCryptoLib(CryptoAlgo algo);

// Шаблон функции получения функции из библиотеки
template <typename Func>
Func loadCryptoFunc(void* handle, const std::string& name) {
    dlerror(); // очистка ошибок
    Func func = reinterpret_cast<Func>(dlsym(handle, name.c_str()));
    const char* dlsym_error = dlerror();
    if (dlsym_error)
        throw std::runtime_error("Не удалось найти функцию " + name + ": " + std::string(dlsym_error));
    return func;
}
