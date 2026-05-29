#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdint.h>

// Функція підрахунку
uint32_t LibraryGetControlSum(uint8_t* pMem, uint32_t dwCount) {
    uint32_t dwCS = 0;
    uint32_t dwFull = dwCount / 4;
    uint32_t* p32Mem = (uint32_t*)pMem;

    for (uint32_t i = 0; i < dwFull; ++i) dwCS += p32Mem[i];

    uint32_t dwNotFull = dwCount % 4;

    if (dwNotFull) {
        pMem += dwFull * 4;
        uint32_t dwNumber = 0;

        std::memcpy(&dwNumber, pMem, dwNotFull);
        dwCS += dwNumber;
    }
    return dwCS;
}

bool VerifySelfControlSum() {
    const char* libPath = "./libmylib.so";
    
    int fd = open(libPath, O_RDONLY);
    if (fd < 0) return false;

    off_t dwFullSize = lseek(fd, 0, SEEK_END);

    if (dwFullSize < 4) {
        close(fd);
        return false;
    }

    off_t dwBodySize = dwFullSize - 4;
    lseek(fd, 0, SEEK_SET);

    uint8_t* pMem = new uint8_t[dwFullSize];

    if (read(fd, pMem, dwFullSize) != dwFullSize) {
        delete[] pMem;
        close(fd);

        return false;
    }

    uint32_t savedCS = *(uint32_t*)(pMem + dwBodySize);

    uint32_t calculatedCS = LibraryGetControlSum(pMem, dwBodySize);

    delete[] pMem;
    close(fd);

    return savedCS == calculatedCS;
}

// Аналог DllMain 
__attribute__((constructor))
void OnLibraryLoad() {

    std::cout << "[БІБЛІОТЕКА] Перевірка цілісності...\n";

    if (VerifySelfControlSum()) {
        std::cout << "[БІБЛІОТЕКА] Контрольна сума збігається. Доступ дозволено.\n";

    } else {
        std::cerr << "[КРИТИЧНА ПОМИЛКА] Контрольна сума пошкоджена або модифікована!\n";

        std::exit(EXIT_FAILURE); 
    }
}

// Демонстраційна функція бібліотеки
extern "C" void HelloFromLibrary() {
    std::cout << "Функція працює.\n";
}