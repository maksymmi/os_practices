#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>

using namespace std;

struct Owner {

    char surname[30];
    char name[30];
    char middleName[30];
    char carNumber[15];
    char phone[20];
    char techPassport[20];
};

Owner inputOwner() {

    Owner o;

    cout << "Прізвище: ";
    cin >> o.surname;

    cout << "Ім'я: ";
    cin >> o.name;

    cout << "Побатькові: ";
    cin >> o.middleName;

    cout << "Номер машини: ";
    cin >> o.carNumber;

    cout << "Телефон: ";
    cin >> o.phone;

    cout << "Номер техпаспорту: ";
    cin >> o.techPassport;

    return o;
}

void printOwner(const Owner& o) {

    cout << o.surname << " "
         << o.name << " "
         << o.middleName<< " | "
         << o.carNumber << " | "
         << o.phone << " | "
         << o.techPassport << endl;
}

// C
const char* FILE_C = "owners_c.txt";

void createFileC() {

    FILE* f = fopen(FILE_C, "w");
    int n;

    cout << "C: скільки записів? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nRecord " << i + 1 << endl;
        Owner o = inputOwner();
        fwrite(&o, sizeof(Owner), 1, f);
    }

    fclose(f);
}

void showFileC() {

    FILE* f = fopen(FILE_C, "r");
    Owner o;

    cout << "\n--- C Файл ---\n";

    while (fread(&o, sizeof(Owner), 1, f)) {
        printOwner(o);
    }

    fclose(f);
}

void deleteByCarC(const char* carNumber) {

    FILE* f = fopen(FILE_C, "r");
    FILE* temp = fopen("temp.txt", "w");

    Owner o;

    while (fread(&o, sizeof(Owner), 1, f)) {

        if (strcmp(o.carNumber, carNumber) != 0) {
            fwrite(&o, sizeof(Owner), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove(FILE_C);
    rename("temp.txt", FILE_C);
}

void addByK_C(const char* k, int N) {

    FILE* f = fopen(FILE_C, "a");

    for (int i = 0; i < N; i++) {

        Owner o = inputOwner();

        strcpy(o.carNumber, k);
        fwrite(&o, sizeof(Owner), 1, f);
    }

    fclose(f);
}

// C++ 
const char* FILE_CPP = "owners_cpp.txt";

void createFileCPP() {
    ofstream f(FILE_CPP, ios::binary);

    int n;
    cout << "C++: скільки записів? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nRecord " << i + 1 << endl;
        Owner o = inputOwner();
        f.write((char*)&o, sizeof(Owner));
    }

    f.close();
}

void showFileCPP() {
    ifstream f(FILE_CPP, ios::binary);
    Owner o;

    cout << "\n--- C++ Файл ---\n";

    while (f.read((char*)&o, sizeof(Owner))) {
        printOwner(o);
    }

    f.close();
}

void deleteByCarCPP(const char* carNumber) {
    ifstream f(FILE_CPP, ios::binary);
    ofstream temp("temp_cpp.txt", ios::binary);

    Owner o;

    while (f.read((char*)&o, sizeof(Owner))) {
        if (strcmp(o.carNumber, carNumber) != 0) {
            temp.write((char*)&o, sizeof(Owner));
        }
    }

    f.close();
    temp.close();

    remove(FILE_CPP);
    rename("temp_cpp.txt", FILE_CPP);
}

void addByK_CPP(const char* k, int N) {
    ofstream f(FILE_CPP, ios::binary | ios::app);

    for (int i = 0; i < N; i++) {
        Owner o = inputOwner();
        strcpy(o.carNumber, k);
        f.write((char*)&o, sizeof(Owner));
    }

    f.close();
}

int main() {
    setlocale(LC_ALL, "Ukrainian");

    int choice;

    do {
        cout << "\n================ Меню ================\n";
        cout << "1. Створити файл (C)\n";
        cout << "2. Надрукувати файл (C)\n";
        cout << "3. Видалити за номером машини (C)\n";
        cout << "4. Додати N елементів з K (C)\n";
        cout << "5. Створити файл (C++)\n";
        cout << "6. Надрукувати файл (C++)\n";
        cout << "7. Видалити за номером машини (C++)\n";
        cout << "8. Додати N елементів з K (C++)\n";
        cout << "0. Завершити роботу\n";
        cout << "Вибір -> ";
        cin >> choice;

        if (choice == 1) createFileC();
        else if (choice == 2) showFileC();
        else if (choice == 3) {

            char k[15];
            cout << "Ввадіть номер машини для видалення: ";
            cin >> k;

            deleteByCarC(k);
        }
        else if (choice == 4) {

            char k[15];
            int n;

            cout << "Номер машини K: ";
            cin >> k;
            cout << "N: ";
            cin >> n;

            addByK_C(k, n);
        }
        else if (choice == 5) createFileCPP();
        else if (choice == 6) showFileCPP();
        else if (choice == 7) {

            char k[15];

            cout << "Ввадіть номер машини для видалення: ";
            cin >> k;

            deleteByCarCPP(k);
        }
        else if (choice == 8) {

            char k[15];
            int n;

            cout << "Номер машини K: ";
            cin >> k;
            cout << "N: ";
            cin >> n;

            addByK_CPP(k, n);
        }

    } while (choice != 0);

    return 0;
}