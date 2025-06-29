# РГР — Консольное приложение для шифрования и дешифрования

## Описание
Данный проект представляет собой консольное приложение для шифрования и дешифрования текста и файлов с использованием различных алгоритмов:
- **Табличная перестановка**
- **AES-128 CFB**
- **Шифр Виженера**

## Возможности

- Шифрование и дешифрование текста и файлов
- Поддержка всех алфавитов
- Выбор алгоритма шифрования
- Автоматическая запись ключа в файл
- Реализация под MacOS/Linux

## Установка

1. Клонируйте репозиторий:
   ```
   git clone https://github.com/ZeroNSK/Crypto
   ```
2. Перейдите в директорию проекта:
   ```
   cd Crypto
   ```
3. Установите зависимости и соберите проект:
   ```
   sudo chmod +x install.sh
   sudo ./install.sh
   ```

## Запуск

Для запуска приложения используйте команду:
```
crypto
```

## Структура проекта

```cpp
.
├── build
    ├── crypto
    └── libaes_cipher.dylib
    └── libtable_cipher.dylib
    └── libvigener_cipher.dylib
├── lib
│   ├── cypher_interface.h
│   ├── table
│   │   ├── table.cpp
│   ├── Aes
│   │   ├── aes.cpp <- aes.h
│   │   ├── cipher1.cpp <- aes.h, aes.cpp
│   │   └── aes.h
│   ├── Viginer
│   │   ├── viginer.cpp
└── src
    └── main.cpp <- general.h
    └── general.cpp <- general.h
    └── general.h <- cypher_interface.h
├── makefile
├── install.sh
├── README.md


6 directories, 13 files
```
