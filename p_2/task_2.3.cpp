// Завдання 2.3

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int MAX_ROWS = 10;
const int MAX_COLS = 10;

int main() {
    setlocale(LC_ALL, "Ukrainian");
    srand(static_cast<unsigned>(time(NULL)));

    int matrix[MAX_ROWS][MAX_COLS];
    int rows, cols;

    cout << "Введіть кількість рядків та стовпців (до 10) -> ";
    cin >> rows >> cols;

    if (rows > MAX_ROWS) rows = MAX_ROWS;
    if (cols > MAX_COLS) cols = MAX_COLS;

    cout << "Сформована матриця:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100 - 50;
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }

    int sum = 0;
    bool found = false;

    int size = (rows < cols) ? rows : cols; 

    // Розрахунок діагоналі
    for (int i = 0; i < size; i++) {
        int current = matrix[i][size - 1 - i];
        
        if (current < 0 && current % 2 != 0) {
            sum += current;
            found = true;
        }
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    if (found) {
        cout << "Сума негативних непарних чисел побічної діагоналі -> " << sum << endl;
    } else {
        cout << "Шуканих чисел на побічній діагоналі не знайдено." << endl;
    }

    return 0;
}