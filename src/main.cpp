#include "general.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

typedef void* (*EncryptFunc)(const unsigned char*, size_t, int, int, size_t*);
typedef void (*FreeFunc)(void*);

// Считывание файла в вектор
vector<unsigned char> readFile(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) throw runtime_error("Не удалось открыть файл " + filename);
    return vector<unsigned char>((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
}

// Запись вектора в файл
void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream out(filename, ios::binary);
    if (!out) throw runtime_error("Не удалось записать файл " + filename);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Ввод строки -> вектор
vector<unsigned char> readText() {
    string line;
    cout << "Введите текст: ";
    cin.ignore();
    getline(cin, line);
    return vector<unsigned char>(line.begin(), line.end());
}

// Вывод результата на экран
void printText(const vector<unsigned char>& data) {
    cout << "Результат: ";
    for (auto c : data) cout << c;
    cout << "\n";
}

// Главное меню
void showMenu() {
    cout << "\033[2J\033[H"; 
    cout << "Действие:\n"
         << "\033[32m1. Зашифровать текст\033[0m\n"
         << "\033[31m2. Расшифровать текст\033[0m\n"
         << "\033[34m3. Зашифровать файл\033[0m\n"
         << "\033[35m4. Расшифровать файл\033[0m\n"
         << "\033[90m0. Выход\033[0m\n"
         << "\nВыбор: ";
}


// Выбор алгоритма
CryptoAlgo selectAlgorithm() {
    int c;
    cout << "Алгоритмы:\n"
         << "1. Табличная перестановка\n"
         << "2. AES\n"
         << "3. Виженер\n"
         << "Выбор: ";
    cin >> c;
    if (c >= 1 && c <= 3) return static_cast<CryptoAlgo>(c);
    throw runtime_error("Неверный выбор алгоритма.");
}

// Запуск шифрования/дешифрования
void runOperation(bool encrypt, bool isFile) {
    try {
        CryptoAlgo algo = selectAlgorithm();
        string color;
        switch (algo) {
        case TABLE:   color = "\033[33m"; break; // жёлтый
        case AES:     color = "\033[34m"; break; // синий
        case VIGENER: color = "\033[35m"; break; // фиолетовый
}
        string algoName = (algo == TABLE) ? "Табличная перестановка" : (algo == AES ? "AES" : "Виженер");
        cout << color << "🛡️  Алгоритм выбран: " << algoName << "\033[0m\n";

        void* lib = loadCryptoLib(algo);
        auto func = loadCryptoFunc<EncryptFunc>(lib, encrypt ? "encrypt" : "decrypt");
        auto freeBuf = loadCryptoFunc<FreeFunc>(lib, "free_buffer");

        vector<unsigned char> input;
        if (isFile) {
            string filename;
            cout << "Введите имя входного файла: ";
            cin >> filename;
            input = readFile(filename);
        } else {
            input = readText();
        }

        // Параметры шифра
        int param1 = 0, param2 = 0;
        if (algo == TABLE) {
            cout << "Введите 0 для случайных строк/столбцов, или 1 — чтобы задать вручную: ";
            int mode;
            cin >> mode;
            if (mode == 0) {
                srand(time(nullptr));
                param1 = rand() % 5 + 2; // строки [2..6]
                param2 = rand() % 5 + 2; // столбцы [2..6]
                cout << "🔁 Случайные параметры: " << param1 << " строк × " << param2 << " столбцов\n";
            } else {
                cout << "Введите количество строк: "; cin >> param1;
                cout << "Введите количество столбцов: "; cin >> param2;
            }
        }
        else if (algo == VIGENER) {
            cout << "1 — автогенерация ключа, 0 — вручную по seed: ";
            cin >> param1;
            if (param1 == 0) {
                cout << "Введите seed: ";
                cin >> param2;
            }
        }

        size_t outSize;
        void* result = func(input.data(), input.size(), param1, param2, &outSize);

        vector<unsigned char> output((unsigned char*)result, (unsigned char*)result + outSize);
        freeBuf(result);

        if (isFile) {
            string outFile;
            cout << "Введите имя выходного файла: ";
            cin >> outFile;
            writeFile(outFile, output);
            cout << "Файл сохранён: " << outFile << "\n";
        } else {
            printText(output);
        }

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }
    cout << "\nНажмите Enter для продолжения...";
    cin.ignore();
    cin.get();
    cout << "\033[2J\033[H"; 

}

int main() {
    cout << "\033[2J\033[H";
    setlocale(LC_ALL, "");

    while (true) {
        showMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: runOperation(true, false); break;
            case 2: runOperation(false, false); break;
            case 3: runOperation(true, true); break;
            case 4: runOperation(false, true); break;
            case 0: cout << "Выход.\n"; return 0;
            default: cout << "Неверный выбор.\n";
        }
    }
}
