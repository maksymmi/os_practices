#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <clocale>
#include <locale>
#include <cwchar>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <codecvt>

#ifdef UNICODE
    typedef wchar_t TCHAR_SIM;
    #define TEXT_SIM(x) L ## x
#else
    typedef char TCHAR_SIM;
    #define TEXT_SIM(x) x
#endif

int compare_wchar(const void* a, const void* b) {
    const wchar_t* strA = *(const wchar_t**)a;
    const wchar_t* strB = *(const wchar_t**)b;

    return wcscmp(strA, strB);
}

int main() {
    // Налаштування локалі 
    std::setlocale(LC_ALL, ""); 
    std::locale loc("");
    std::locale::global(loc);
    std::wcout.imbue(loc);

    std::wcout << L"Перевірка кодування\n";
    std::wcout << L"Розмір типу TCHAR_SIM -> " << sizeof(TCHAR_SIM) << L" байт(ів).\n";

    #ifdef UNICODE
        std::wcout << L"Режим компіляції -> UNICODE\n";
    #else
        std::wcout << L"Режим компіляції -> ASCII / MultiByte\n";
    #endif

    std::vector<std::string> family_ascii = {
        "Варениченко Ничипір Іванович",
        "Ґудзь Галя Максимівна",
        "Боруля Степан Мартинович",
        "Кайдаш Карпо Омелькович"
    };

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    std::wcout << L"\nВивід масиву ASCII\n";

    for (const auto& name : family_ascii) {
        std::wcout << converter.from_bytes(name) << L"\n";
    }

    std::vector<std::wstring> family_unicode;

    for (const auto& name : family_ascii) {
        family_unicode.push_back(converter.from_bytes(name));
    }

    std::wcout << L"\nВивід масиву UNICODE\n";
    std::wcout << L"\n[wprintf]:\n";

    for (const auto& name : family_unicode) {
        wprintf(L"%ls\n", name.c_str());
    }

    std::wcout << L"\n[wcout]:\n";

    for (const auto& name : family_unicode) {
        std::wcout << name << L"\n";
    }

    std::wcout << L"\n[MessageBox симуляція.?] ->\n";
    std::wcout << L"| Повідомлення -> " << family_unicode[0] << L" |\n";

    std::vector<const wchar_t*> qsort_arr;

    for (const auto& name : family_unicode) {
        qsort_arr.push_back(name.c_str());
    }

    qsort(qsort_arr.data(), qsort_arr.size(), sizeof(const wchar_t*), compare_wchar);

    std::vector<std::wstring> family_stdsort = family_unicode;
    std::sort(family_stdsort.begin(), family_stdsort.end());

    std::wcout << L"\nВідсортований масив UNICODE (std::sort)\n";

    for (const auto& name : family_stdsort) {
        std::wcout << name << L"\n";
    }

    std::vector<std::string> family_ascii_sorted;

    for (const auto& name : family_stdsort) {
        family_ascii_sorted.push_back(converter.to_bytes(name));
    }

    std::wcout << L"\nРезультат зворотнього перетворення(ASCII)\n";

    for (const auto& name : family_ascii_sorted) {
        std::wcout << converter.from_bytes(name) << L"\n";
    }

    return 0;
}
#pragma GCC diagnostic pop