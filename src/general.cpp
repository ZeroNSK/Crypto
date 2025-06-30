#include "general.h"

void* loadCryptoLib(CryptoAlgo algo) {
    std::string path;
#ifdef __APPLE__
    const std::string ext = ".dylib";
#else
    const std::string ext = ".so";
#endif

switch (algo) {
    case TABLE:   path = "/usr/local/lib/crypto_libs/libtable_cipher" + ext; break;
    case AES:     path = "/usr/local/lib/crypto_libs/libaes_cipher" + ext; break;
    case VIGENER: path = "/usr/local/lib/crypto_libs/libvigener_cipher" + ext; break;
    default: throw std::runtime_error("Неизвестный алгоритм");
    }

    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
        throw std::runtime_error("Не удалось загрузить " + path + ": " + dlerror());
    return handle;
}
