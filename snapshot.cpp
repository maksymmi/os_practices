#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

void PrintSharedLibraries(const std::string& pid) {
    std::string mapsPath = "/proc/" + pid + "/maps";
    std::ifstream mapsFile(mapsPath);

    if (!mapsFile.is_open()) {
        std::cout << "      [Доступ до списку .so обмежено ОС]\n";

        return;
    }

    std::cout << "      Завантажені бібліотеки (.so):\n";
    std::string line;

    int counter = 0;
    std::string lastSo = "";

    while (std::getline(mapsFile, line)) {
        size_t pos = line.find("/");

        if (pos != std::string::npos && line.find(".so") != std::string::npos) {
            std::string soPath = line.substr(pos);

            if (soPath != lastSo) {
                std::cout << "        -> " << soPath << "\n";
                lastSo = soPath;

                counter++;
            }
        }
        if (counter >= 4) {
            std::cout << "        -> ~ та інші спільні бібліотеки\n";

            break;
        }
    }
}

int main() {
    std::cout << "Завдання 3: Системний знімок процесів ~\n\n";

    DIR* procDir = opendir("/proc");
    if (!procDir) {
        std::cerr << "Не вдалося відкрити /proc\n";

        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(procDir)) != NULL) {
        bool isPid = true;

        for (int i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit(entry->d_name[i])) {
                isPid = false;

                break;
            }
        }

        if (isPid) {
            std::string pid = entry->d_name;
            std::ifstream commFile("/proc/" + pid + "/comm");

            std::string procName;
            std::getline(commFile, procName);

            int threadCount = 0;
            DIR* taskDir = opendir(("/proc/" + pid + "/task").c_str());

            if (taskDir) {
                while (readdir(taskDir)) threadCount++;

                threadCount -= 2;
                closedir(taskDir);
            }

            std::cout << "============================================================\n";
            std::cout << "Ім'я процесу ->     " << procName << "\n";
            std::cout << "Ідентифікатор PID -> " << pid << "\n";
            std::cout << "Кількість потоків -> " << threadCount << "\n";

            PrintSharedLibraries(pid);
        }
    }

    closedir(procDir);

    return 0;
}