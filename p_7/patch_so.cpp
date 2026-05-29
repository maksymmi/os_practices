#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdint.h>

// Підрахунок (POSIX)
uint32_t GetControlSum(uint8_t* pMem, uint32_t dwCount) {
    uint32_t dwCS = 0;
    uint32_t dwFull = dwCount / 4;
    uint32_t* p32Mem = (uint32_t*)pMem;

    for (uint32_t i = 0; i < dwFull; ++i) {
        dwCS += p32Mem[i];
    }

    uint32_t dwNotFull = dwCount % 4;

    if (dwNotFull) {
        pMem += dwFull * 4;
        uint32_t dwNumber = 0;

        std::memcpy(&dwNumber, pMem, dwNotFull);
        dwCS += dwNumber;
    }
    return dwCS;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Використання: " << argv << " <шлях_до_бібліотеки.so>\n";

        return 1;
    }

    const char* soPath = argv[1];

    int fd = open(soPath, O_RDWR);
    if (fd < 0) {
        std::perror("Не вдалося відкрити .so файл");

        return 1;
    }

    // Розмір файлу через lseek
    off_t dwSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    uint8_t* pMem = new uint8_t[dwSize];

    if (read(fd, pMem, dwSize) != dwSize) {
        std::perror("Помилка читання файлу");
        delete[] pMem;
        close(fd);

        return 1;
    }

    // Контрольна сума
    uint32_t dwCS = GetControlSum(pMem, dwSize);

    lseek(fd, 0, SEEK_END);

    if (write(fd, &dwCS, sizeof(dwCS)) != sizeof(dwCS)) {
        std::perror("Помилка запису контрольної суми");
        delete[] pMem;
        close(fd);

        return 1;
    }

    std::cout << "[УСПІХ] Для " << soPath << " записано КС: 0x" << std::hex << dwCS << std::endl;

    delete[] pMem;
    close(fd);

    return 0;
}