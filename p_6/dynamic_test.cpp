#include <iostream>
#include <dlfcn.h> // POSIX стандарт для динамічного завантаження

typedef bool (*arith_func)(long long, long long, long long*);

int main() {
    // Завантажуємо бібліотеку (аналог LoadLibrary)
    void* handle = dlopen("./libarithmetic.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Не вдалося завантажити бібліотеку: " << dlerror() << "\n";
        return 1;
    }

    // Отримуємо адресу функції (аналог GetProcAddress)
    arith_func mul_safe = (arith_func)dlsym(handle, "mul_safe");
    
    const char* dlsym_error = dlerror();

    if (dlsym_error) {
        std::cerr << "Не вдалося знайти функцію: " << dlsym_error << "\n";
        dlclose(handle);
        return 1;
    }

    long long res;

    if (mul_safe(1000000000LL, 1000000000LL, &res)) {
        std::cout << "Динамічне завантаження: Переповнення!\n";

    } else {
        std::cout << "Динамічне завантаження: Результат = " << res << "\n";
    }

    // Закриваємо бібліотеку (аналог FreeLibrary)
    dlclose(handle);

    return 0;
}