#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>

enum class Encoding { ASCII_UTF8, UTF16_LE, UTF16_BE, UTF32_LE };

// 2. Aвтовизначення кодування
Encoding DetectEncoding(const std::string& raw_bytes) {

    if (raw_bytes.size() >= 4) {

        if (raw_bytes[0] == '\xFF' && raw_bytes[1] == '\xFE' && raw_bytes[2] == '\x00' && raw_bytes[3] == '\x00')

            return Encoding::UTF32_LE;

    }
    if (raw_bytes.size() >= 2) {

        if (raw_bytes[0] == '\xFF' && raw_bytes[1] == '\xFE') return Encoding::UTF16_LE;

        if (raw_bytes[0] == '\xFE' && raw_bytes[1] == '\xFF') return Encoding::UTF16_BE;
    }
    
    return Encoding::ASCII_UTF8; 
}

void SortUnknownEncoding(std::vector<std::string>& raw_strings) {
    std::locale loc("");
    std::vector<std::string> utf8_strings;

    for (const auto& raw : raw_strings) {
        Encoding enc = DetectEncoding(raw);
        
        if (enc == Encoding::ASCII_UTF8) {
            utf8_strings.push_back(raw);
        } 

        else if (enc == Encoding::UTF16_LE) {
            // Перетворюємо UTF-16LE в UTF-8
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

            const char16_t* p = reinterpret_cast<const char16_t*>(raw.data());

            // Обрізаємо BOM якщо є
            if (*p == 0xFEFF) p++; 

            utf8_strings.push_back(convert.to_bytes(p));
        }
    }

    // Сортуємо масив UTF-8 
    std::sort(utf8_strings.begin(), utf8_strings.end(), loc);

    raw_strings = utf8_strings;
}

int main() {
    std::locale::global(std::locale(""));

    std::vector<std::string> unknown_dataset;
    
    // UTF-8 рядок
    unknown_dataset.push_back("Харків");
    
    // UTF-16LE рядок
    std::u16string wstr = u"Київ";
    std::string raw_u16(reinterpret_cast<const char*>(wstr.data()), wstr.size() * 2);

    unknown_dataset.push_back(raw_u16);

    SortUnknownEncoding(unknown_dataset);

    std::cout << "Успішно нормалізовано та відсортовано:\n";

    for (const auto& s : unknown_dataset) {
        std::cout << s << "\n";
    }

    return 0;
}