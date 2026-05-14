#include <iostream>
#include <cstring> 

using namespace std;

struct person {
    char* name;
    int age;
};

person make_person() {

    int Age;
    char Name[20];

    cout << "Введіть ім'я -> ";
    cin >> Name;
    cout << "Введіть вік -> ";
    cin >> Age;

    person p;
    p.name = new char[strlen(Name) + 1];
    strcpy(p.name, Name);
    p.age = Age;

    return p;
}

void print_person(person p) {
    cout << "\nІм'я: " << p.name << "\tВік: " << p.age;
}

person* filter_names_by_A(person* source, int size, int& newSize) {

    newSize = 0;

    for (int i = 0; i < size; i++) {

        if (source[i].name[0] == 'A' || source[i].name[0] == 'a') {
            newSize++;
        }
    }

    if (newSize == 0) return nullptr;

    person* result = new person[newSize];
    int j = 0;

    for (int i = 0; i < size; i++) {

        if (source[i].name[0] == 'A' || source[i].name[0] == 'a') {

            result[j].name = new char[strlen(source[i].name) + 1];
            strcpy(result[j].name, source[i].name);
            result[j].age = source[i].age;

            j++;
        }
    }
    return result;
}

void delete_even_rows(char**& arr, int& count) {

    if (count == 0) {

        cout << "\nПомилка: Спроба видалення з порожнього масиву!" << endl;
        return;
    }

    int newCount = (count + 1) / 2; 
    char** newArr = new char*[newCount];

    int j = 0;

    for (int i = 0; i < count; i++) {

        if (i % 2 == 0) { 
            newArr[j] = arr[i]; 
            j++;
        } else {
            delete[] arr[i]; 
        }
    }

    delete[] arr; 
    arr = newArr;
    count = newCount;
}

int main() {
    setlocale(LC_ALL, "Ukrainian");

    // 1: Масив структур
    int n;
    cout << "Скільки людей додати в масив? ";
    cin >> n;

    person* group = new person[n];
    for (int i = 0; i < n; i++) {
        cout << "Людина #" << i + 1 << ":" << endl;
        group[i] = make_person();
    }

    cout << "\nПочатковий масив структур:";
    for (int i = 0; i < n; i++) print_person(group[i]);

    int filteredSize;
    person* filteredGroup = filter_names_by_A(group, n, filteredSize);

    if (filteredGroup == nullptr) {
        cout << "\n\nЕлементи, що починаються на 'A', не знайдені.";
    } else {
        cout << "\n\nМасив людей на літеру 'A':";
        for (int i = 0; i < filteredSize; i++) print_person(filteredGroup[i]);
    }

    // 2: динамічні рядки
    int strCount;

    cout << "\n\nСкільки рядків ввести? ";
    cin >> strCount;

    char** lines = new char*[strCount];

    for (int i = 0; i < strCount; i++) {

        char buffer[100];

        cout << "Введіть рядок " << i + 1 << ": ";
        cin >> buffer;

        lines[i] = new char[strlen(buffer) + 1];
        strcpy(lines[i], buffer);
    }

    cout << "\nМасив рядків до обробки:" << endl;
    for (int i = 0; i < strCount; i++) cout << i + 1 << ": " << lines[i] << endl;

    delete_even_rows(lines, strCount);

    cout << "\nМасив рядків після видалення парних:" << endl;
    for (int i = 0; i < strCount; i++) cout << i + 1 << ": " << lines[i] << endl;

    for (int i = 0; i < n; i++) delete[] group[i].name;
    delete[] group;
    
    if (filteredGroup) {
        for (int i = 0; i < filteredSize; i++) delete[] filteredGroup[i].name;
        delete[] filteredGroup;
    }

    for (int i = 0; i < strCount; i++) delete[] lines[i];
    delete[] lines;

    return 0;
}