#include <iostream>

extern "C" {
    bool add_safe(long long a, long long b, long long* res);
}

int main() {
    long long a = 9223372036854775807LL; // Максимум для 64 біт
    long long b = 1;
    long long result;

    if (add_safe(a, b, &result)) {
        std::cout << "Помилка: Переповнення при додаванні!\n";
    } else {
        std::cout << "Результат: " << result << "\n";
    }
    return 0;
}