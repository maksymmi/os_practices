#include <iostream>
#include <dlfcn.h> 
#include <cstdint>

typedef uint64_t (*pGenerateE)(uint64_t);
typedef uint64_t (*pGenerateD)(uint64_t, uint64_t);
typedef uint64_t (*pRsaCrypt)(uint64_t, uint64_t, uint64_t);
typedef const char* (*pGetString)(int);

int main() {
    std::cout << "Оберіть мову / Choose language (1 - Українська, 2 - English) -> ";
    int langChoice;
    std::cin >> langChoice;

    const char* resSoName = (langChoice == 1) ? "./liblang_ua.so" : "./liblang_en.so";
    
    // Завантаження бібліотеки (aналог LoadLibrary)
    void* hResLib = dlopen(resSoName, RTLD_LAZY);
    if (!hResLib) {
        std::cerr << "Помилка завантаження бібліотеки: " << dlerror() << "\n";

        return 1;
    }

    pGetString get_string = (pGetString)dlsym(hResLib, "get_string");
    
    std::cout << "\nІнформація ->\n";
    if (get_string) {
        for (int i = 101; i <= 104; ++i) {
            std::cout << get_string(i) << "\n";
        }
    }
    dlclose(hResLib); // замість FreeLibrary

    // завантаження RSA SO 
    std::cout << "\nЗавантаження криптографічної бібліотеки...\n";
    
    void* hRsaLib = dlopen("./librsa_lib.so", RTLD_LAZY);
    if (!hRsaLib) {
        std::cerr << "Не вдалося завантажити :/ librsa_lib.so\n";

        return 1;
    }

    pGenerateE generate_e = (pGenerateE)dlsym(hRsaLib, "generate_e");
    pGenerateD generate_d = (pGenerateD)dlsym(hRsaLib, "generate_d");
    pRsaCrypt rsa_crypt = (pRsaCrypt)dlsym(hRsaLib, "rsa_crypt");

    if (!generate_e || !generate_d || !rsa_crypt) {
        std::cerr << "Не вдалося знайти адреси функцій.\n";
        dlclose(hRsaLib);

        return 1;
    }

    uint64_t p = 61, q = 53;
    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);
    
    uint64_t e_key = generate_e(phi);
    uint64_t d_key = generate_d(e_key, phi);

    std::cout << "Згенеровано ключі -> n=" << n << ", e=" << e_key << ", d=" << d_key << "\n";
    uint64_t original_msg = 2026;
    
    std::cout << "Оригінальне повідомлення -> " << original_msg << "\n";
    uint64_t encrypted = rsa_crypt(original_msg, e_key, n);
    std::cout << "Зашифровано -> " << encrypted << "\n";
    uint64_t decrypted = rsa_crypt(encrypted, d_key, n);
    std::cout << "Розшифровано -> " << decrypted << "\n";

    if (original_msg == decrypted) {
        std::cout << "-> Успіх! Дані збігаються.\n\n";
    }

    dlclose(hRsaLib);
    return 0;
}