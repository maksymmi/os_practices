#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    std::cout << "Програма 1: Запуск текстового редактора ~\n";

    std::string folderPath;
    if (argc > 1) {
        folderPath = argv[1];

    } else {
        const char* envOpt = getenv("LAB7_FOLDER");
        folderPath = envOpt ? envOpt : "/tmp/Lab7Folder";
    }

    mkdir(folderPath.c_str(), 0777);
    std::cout << "Робоча директорія -> " << folderPath << "\n";

    std::string filePath = folderPath + "/file.txt";

    // Створення та запис звичайного файлу
    std::ofstream file(filePath);

    if (file.is_open()) {
        std::string text = "Це стандартний текстовий файл.\nПерший рядок тексту.\nКінець.";

        file.write(text.c_str(), text.size());
        file.close();
    }

    std::cout << "Запускаємо редактор nano для перегляду створеного файлу ~\n";
    std::string cmd = "nano " + filePath;

    system(cmd.c_str()); // Відкриваємо nano

    return 0;
}