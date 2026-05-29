#include <iostream>
#include <string>
#include <cwctype>
#include <locale>
#include <codecvt>

// Видаляємо '/' або '\' та переводимо у нижній регістр UTF-32
std::wstring NormalizeAndLower(const std::string& utf8_path) {
    // UTF-8 (std::string) у UTF-32 (std::wstring для linux GCC)
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wpath = converter.from_bytes(utf8_path);

    if (!wpath.empty() && (wpath.back() == L'/' || wpath.back() == L'\\')) {
        wpath.pop_back();
    }

    // Юнікод-символ -> нижній регістр
    for (auto& ch : wpath) {
        ch = std::towlower(ch);
    }
    return wpath;
}

bool CompareDirectories(const std::string& path1, const std::string& path2) {
    // Системну локаль 
    std::locale::global(std::locale(""));
    
    return NormalizeAndLower(path1) == NormalizeAndLower(path2);
}

int main() {
    std::string dir1 = "/home/user/Документи/";
    std::string dir2 = "/home/user/ДОКУМЕНТИ"; // Без слешу

    if (CompareDirectories(dir1, dir2)) {
        std::cout << "Успіх! Каталоги однакові\n";
    } else {
        std::cout << "Помилка! Каталоги різні\n";
    }
    return 0;
}