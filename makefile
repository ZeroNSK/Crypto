CXX = g++
CXXFLAGS = -std=c++17 -Wall -fPIC -I./lib -I./src
LDFLAGS = -ldl

SRC_DIR = src
BUILD_DIR = build

# Автоопределение .so/.dylib
EXT = .so
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    EXT = .dylib
endif

USER_ID := $(shell id -u)

# Исходники шифров
TABLE_SRC = lib/table/table.cpp
VIG_SRC   = lib/viginer/viginer.cpp
AES_SRC   = lib/aes/cipher1.cpp lib/aes/aes.cpp

# Цели библиотек
SO_TABLE = $(BUILD_DIR)/libtable_cipher$(EXT)
SO_VIG   = $(BUILD_DIR)/libvigener_cipher$(EXT)
SO_AES   = $(BUILD_DIR)/libaes_cipher$(EXT)
SO_FILES = $(SO_TABLE) $(SO_VIG) $(SO_AES)

# Главный бинарник
MAIN_BIN = $(BUILD_DIR)/crypto

all: $(MAIN_BIN) $(SO_FILES)

# Сборка основного приложения
$(MAIN_BIN): $(SRC_DIR)/main.cpp $(SRC_DIR)/general.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Сборка каждой библиотеки
$(SO_TABLE): $(TABLE_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

$(SO_VIG): $(VIG_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

$(SO_AES): $(AES_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

# Создание каталога сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SO_FILES): | $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
