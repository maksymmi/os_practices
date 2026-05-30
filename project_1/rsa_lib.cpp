#include <iostream>
#include <cstdint>
#include <utility>

// Аналог DllMain 
__attribute__((constructor)) void on_library_load() {
    std::cout << "Бібліотеку завантажено в пам'ять.\n";
}

__attribute__((destructor)) void on_library_unload() {
    std::cout << "Бібліотеку вивантажено.\n";
}

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) { a %= b; std::swap(a, b); }

    return a;
}

uint64_t mod_inverse(uint64_t e, uint64_t phi) {
    int64_t t = 0, newt = 1, r = phi, newr = e;

    while (newr != 0) {
        int64_t quotient = r / newr;

        t = std::exchange(newt, t - quotient * newt);
        r = std::exchange(newr, r - quotient * newr);
    }

    if (r > 1) return 0;
    if (t < 0) t += phi;

    return t;
}

uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) result = (uint64_t)(((unsigned __int128)result * base) % mod);

        exp = exp >> 1;

        base = (uint64_t)(((unsigned __int128)base * base) % mod);
    }

    return result;
}

extern "C" {
    uint64_t generate_e(uint64_t phi) {
        uint64_t e = 65537;
        if (gcd(e, phi) == 1) return e;

        for (e = 3; e < phi; e += 2) {
            if (gcd(e, phi) == 1) return e;
        }

        return 0;
    }

    uint64_t generate_d(uint64_t e, uint64_t phi) { return mod_inverse(e, phi); }
    uint64_t rsa_crypt(uint64_t data, uint64_t key, uint64_t n) { return mod_exp(data, key, n); }
}