#include <iostream>
#include <dlfcn.h>

int main() {
    std::cout << "[ПРОГРАМА] Запуск системи~\n";
    
    // Завантажуємо .so бібліотеку
    void* handle = dlopen("./libmylib.so", RTLD_NOW);

    if (!handle) {
        std::cerr << "[ПРОГРАМА] Не вдалося завантажити бібліотеку: " << dlerror() << std::endl;

        return 1;
    }

    typedef void (*HelloFunc)();

    HelloFunc func = (HelloFunc)dlsym(handle, "HelloFromLibrary");
    
    if (func) {
        func();
    }

    dlclose(handle);
    std::cout << "[ПРОГРАМА] Завершення роботи.\n";
    return 0;
}