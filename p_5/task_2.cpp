#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>

// Сортування для UTF-8 / ASCII
void SortUTF8(std::vector<std::string>& strings) {
    std::locale loc(""); 
    std::sort(strings.begin(), strings.end(), loc);
}

// UTF-16 (демонстрація сумісності)
void SortUTF16(std::vector<std::u16string>& strings) {
    // Конвертуємо у UTF-8 
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::locale loc("");

    std::sort(strings.begin(), strings.end(), [&](const std::u16string& a, const std::u16string& b) {

        return loc(convert.to_bytes(a), convert.to_bytes(b));
    });
}

int main() {
    std::locale::global(std::locale(""));

    // Демонстрація для UTF-8 / ASCII
    std::vector<std::string> arr8 = {"Яблуко", "Агрус", "Банан", "apple"};

    SortUTF8(arr8);
    std::cout << "Відсортовано UTF-8:\n";

    for (const auto& s : arr8) std::cout << s << " ";

    std::cout << "\n\n";

    // Демонстрація для UTF-16 
    std::vector<std::u16string> arr16 = {u"Чернівці", u"Київ", u"Львів"};
    SortUTF16(arr16);
    
    // Вивід через в UTF-8 для linux
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::cout << "Відсортовано UTF-16:\n";

    for (const auto& s : arr16) std::cout << convert.to_bytes(s) << " ";

    std::cout << "\n";

    return 0;
}