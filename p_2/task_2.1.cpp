// Завдання 2.1

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Kонстанта 
const int MAX_SIZE = 100; 

// виведення масиву
void printArray(int mas[], int n, string message) {
    cout << message << ": ";

    for (int i = 0; i < n; i++) {
        cout << mas[i] << " ";
    }
    cout << endl;
}

int main() {

    setlocale(LC_ALL, "Ukrainian");
    srand(static_cast<unsigned>(time(NULL)));

    int mas[MAX_SIZE];
    int n;

    // 1) Формування масиву
    cout << "Введіть кількість елементів масиву (n < " << MAX_SIZE << "): ";
    cin >> n;

    if (n > MAX_SIZE) n = MAX_SIZE;

    for (int i = 0; i < n; i++) {
        mas[i] = rand() % 100 - 50;
    }

    // 2) Роздрукувати масив
    printArray(mas, n, "Початковий масив");

    // 3) Видалення всіх непарних елементів 
    for (int i = 0; i < n; ) {
        if (mas[i] % 2 != 0) { 
            for (int j = i; j < n - 1; j++) {
                mas[j] = mas[j + 1]; 
            }
            n--;
        } else {
            i++; 
        }
    }

    // 4) Вивести результат
    printArray(mas, n, "Після видалення непарних елементів");

    // 5) Додавання елемента з номером K
    int k, val;
    cout << "Введіть номер K для вставки (0.." << n << ") та значення: ";
    cin >> k >> val;

    if (n < MAX_SIZE && k >= 0 && k <= n) {
        for (int i = n; i > k; i--) {
            mas[i] = mas[i - 1]; 
        }
        mas[k] = val;
        n++;
    }
    // 6) Вивести результат
    printArray(mas, n, "Після додавання елемента");

    // 7) Перестановка на m
    int m;
    cout << "Введіть кількість елементів для зсуву (M): ";
    cin >> m;

    if (n > 0) {
        m = m % n; 
        for (int j = 0; j < m; j++) {
            int last = mas[n - 1];
            for (int i = n - 1; i > 0; i--) {
                mas[i] = mas[i - 1];
            }
            mas[0] = last;
        }
    }
    // 8) Вивести результат
    printArray(mas, n, "Після циклічного зсуву вправо");

    // 9) Пошук елемента, рівного середньому арифметичному
    double sum = 0;

    for (int i = 0; i < n; i++) sum += mas[i];

    int avg = (n > 0) ? static_cast<int>(sum / n) : 0;
    cout << "Середнє арифметичне (ціла частина): " << avg << endl;

    int linearComparisons = 0;
    int foundIndex = -1;

    for (int i = 0; i < n; i++) {
        linearComparisons++;
        if (mas[i] == avg) {
            foundIndex = i;
            break;
        }
    }

    // 10) Вивести результат пошуку
    if (foundIndex != -1) cout << "Елемент знайдено на індексі " << foundIndex << endl;
    else cout << "Елемент не знайдено." << endl;
    cout << "Кількість порівнянь при лінійному пошуку: " << linearComparisons << endl;

    // 11) Сортування методом вибору
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            if (mas[j] < mas[min_idx]) min_idx = j;
        }

        int temp = mas[i];

        mas[i] = mas[min_idx];
        mas[min_idx] = temp;
    }
    // 12) Вивести результат
    printArray(mas, n, "Відсортований масив");

    // 13) Пошук у масиві
    int low = 0, high = n - 1;
    int binaryComparisons = 0;

    bool foundBinary = false;

    while (low <= high) {
        binaryComparisons++;

        int mid = low + (high - low) / 2;
        if (mas[mid] == avg) {
            foundBinary = true;
            break;
        }

        if (mas[mid] < avg) low = mid + 1;
        else high = mid - 1;
    }

    // 14) Вивести результат
    cout << "Бінарний пошук середнього (" << avg << "): " << (foundBinary ? "Знайдено" : "Не знайдено") << endl;

    cout << "Кількість порівнянь при бінарному пошуку: " << binaryComparisons << endl;

    return 0;
}