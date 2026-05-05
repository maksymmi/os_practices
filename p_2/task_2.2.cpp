// Завдання 2.2

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_SIZE = 100;

int main() {
    setlocale(LC_ALL, "Ukrainian");
    srand(static_cast<unsigned>(time(NULL)));

    int mas[MAX_SIZE];
    int n;

    // Введення розміру та формування масиву
    cout << "Введіть кількість елементів n (де n < " << MAX_SIZE << ") -> ";
    cin >> n;

    if (n > MAX_SIZE) n = MAX_SIZE;

    for (int i = 0; i < n; i++) {
        mas[i] = rand() % 100 - 50;
    }

    // Виведення масиву
    cout << "Початковий -> ";
    for (int i = 0; i < n; i++) cout << mas[i] << " ";
    cout << endl;
    
    // Видаляємо коден другий елемент
    int new_n = 0;

    for (int i = 0; i < n; i += 2) {
        mas[new_n] = mas[i];
        new_n++;
    }
    n = new_n; 
    
    cout << "Новий -> ";

    for (int i = 0; i < n; i++) cout << mas[i] << " ";
    cout << endl;

    return 0;
}