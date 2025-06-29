#!/bin/bash

echo "🔧 Установка Шифратора..."

BIN_TARGET="/usr/local/bin/crypto"
LIB_DIR="/usr/local/lib/crypto_libs"

# Расширение библиотеки по системе
EXT="so"
if [[ "$OSTYPE" == "darwin"* ]]; then
  EXT="dylib"
fi

# Проверка root-доступа
if [[ $EUID -ne 0 ]]; then
    echo "❌ Пожалуйста, запустите скрипт с правами root: sudo ./install.sh"
    exit 1
fi

# Очистка и сборка
echo "🧹 Очистка и сборка проекта..."

make clean 2>/dev/null

# Проверка на существующую защищённую папку build/
if [[ -d build ]]; then
    if [[ -f build/crypto && ! -w build/crypto ]]; then
        echo "⚠️  Недостаточно прав для удаления build/: удаляю от root..."
        rm -rf build
    fi
fi

make || { echo "❌ Сборка завершилась с ошибкой!"; exit 1; }

# Проверка бинарника
if [[ ! -f build/crypto ]]; then
    echo "❌ Не найден исполняемый файл build/crypto"
    exit 1
fi

# Проверка библиотек
for lib in libtable_cipher libaes_cipher libvigener_cipher; do
    if [[ ! -f build/${lib}.${EXT} ]]; then
        echo "❌ Не найдена библиотека: ${lib}.${EXT}"
        exit 1
    fi
done

# Установка библиотек
echo "📁 Установка библиотек в $LIB_DIR"
mkdir -p "$LIB_DIR"
cp build/*.${EXT} "$LIB_DIR"

# Установка бинарника
echo "🚀 Установка бинарника в $BIN_TARGET"
cp build/crypto "$BIN_TARGET"
chmod +x "$BIN_TARGET"

echo "✅ Установка завершена! Запускай: crypto"
