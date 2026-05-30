#include <iostream>
#include <vector>
#include "rsa_lib.h"

// Зберігання ключів
struct RSAKeyPair {
    uint64_t e;
    uint64_t d;
    uint64_t n;
};

// Генерації ключів 
RSAKeyPair generate_keys(uint64_t p, uint64_t q) {
    RSAKeyPair keys;

    keys.n = p * q;
    uint64_t phi = (p - 1) * (q - 1);

    keys.e = generate_e(phi);
    keys.d = generate_d(keys.e, phi);

    return keys;
}

int main() {
    std::cout << "~!~|~~~|~!~\n\n";

    // Генеруємо ключі
    RSAKeyPair key0 = generate_keys(61, 53); // p0=61, q0=53
    RSAKeyPair key1 = generate_keys(47, 71); // p1=47, q1=71

    std::cout << "Ключ 0 (n0=" << key0.n << ", e0=" << key0.e << ", d0=" << key0.d << ")\n";
    std::cout << "Ключ 1 (n1=" << key1.n << ", e1=" << key1.e << ", d1=" << key1.d << ")\n\n";

    std::vector<uint64_t> test_data = {42, 123, 2026, 888, 3000};

    for (size_t i = 0; i < test_data.size(); ++i) {
        uint64_t t = test_data[i];

        std::cout << ">>> Оригінальне повідомлення t[" << i << "] = " << t << "\n";

        uint64_t e1t = rsa_crypt(t, key1.e, key1.n);
        std::cout << "  Зашифровано e1 (e1t): " << e1t << "\n";

        uint64_t d1e1t = rsa_crypt(e1t, key1.d, key1.n);
        std::cout << "  Розшифровано d1 (d1e1t): " << d1e1t << "\n";

        if (d1e1t != t) {
            std::cerr << "   ERROR -> Розшифроване значення d1e1t не дорівнює оригіналу t[" << i << "]!\n";

            return 1;
        }

        //  Зашифрування {e0, n0}
        uint64_t e0d1e1t = rsa_crypt(d1e1t, key0.e, key0.n);
        std::cout << "  Зашифровано e0 (e0d1e1t): " << e0d1e1t << "\n";

        // Розшифрування {d0, n0}
        uint64_t d0e0d1e1t = rsa_crypt(e0d1e1t, key0.d, key0.n);
        std::cout << "  Розшифровано d0 (d0e0d1e1t): " << d0e0d1e1t << "\n";

        if (d0e0d1e1t != t) {
            std::cerr << "  ERROR -> Розшифроване значення d0e0d1e1t не дорівнює оригіналу t[" << i << "]!\n";
            return 1;
        }

        std::cout << "  -> Успіх! Дані збігаються.\n\n";
    }

    return 0;
}