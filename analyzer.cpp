#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/resource.h>

void AnalyzeTextFile(const std::string& filePath) {
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "Аналіз файлу -> " << filePath << "\n";

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cout << "Не вдалося відкрити файл.\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::cout << "Розмір файлу -> " << size << " байт\n";

    if (size == 0) {
        std::cout << "Файл порожній.\n";

        return;
    }

    std::vector<char> buffer(size);

    file.read(buffer.data(), size);
    file.close();

    // Прямий підрахунок рядків та довжини
    int lineIndex = 0;
    char* pStart = buffer.data();
    char* pEnd = buffer.data() + size;
    int currentLength = 0;

    while (pStart < pEnd) {
        char ch = *pStart;

        if (ch == '\r') { }
        else if (ch == '\n') {
            lineIndex++;
            std::cout << "Рядок " << lineIndex << " -> довжина = " << currentLength << " симв.\n";
            currentLength = 0;

        } else {
            currentLength++;
        }
        pStart++;
    }
    
    // Якщо файл закінчується без символу переведення рядка
    if (currentLength > 0) {
        lineIndex++;

        std::cout << "Рядок " << lineIndex << " -> довжина = " << currentLength << " симв.\n";
    }
    
    std::cout << "Загальна кількість рядків -> " << lineIndex << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "Програма 2: Аналізатор файлів (Поточний пріоритет nice -> " 
              << getpriority(PRIO_PROCESS, 0) << ") ---\n";

    std::string folderPath;
    if (argc > 1) {
        folderPath = argv[1];

    } else {
        const char* envOpt = getenv("LAB7_FOLDER");
        folderPath = envOpt ? envOpt : "/tmp/Lab7Folder";
    }

    DIR* dir = opendir(folderPath.c_str());
    if (!dir) {
        std::cout << "Папку не знайдено.\n";
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string filename = entry->d_name;

        if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt") {
            AnalyzeTextFile(folderPath + "/" + filename);
        }
    }
    closedir(dir);

    return 0;
}