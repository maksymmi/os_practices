#include <limits>

extern "C" {
    // Повертає true, якщо переповнення відбулося
    bool add_safe(long long a, long long b, long long* res) {
        return __builtin_add_overflow(a, b, res);
    }

    bool sub_safe(long long a, long long b, long long* res) {
        return __builtin_sub_overflow(a, b, res);
    }

    bool mul_safe(long long a, long long b, long long* res) {
        return __builtin_mul_overflow(a, b, res);
    }

    bool div_safe(long long a, long long b, long long* res) {
        if (b == 0) return true; // Ділення на нуль
        if (a == std::numeric_limits<long long>::min() && b == -1) return true; // Overflow
        *res = a / b;
        return false;
    }
}