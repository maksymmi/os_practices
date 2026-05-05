// Завдання 2.4

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

int* form_mas(int n) {
    if (n <= 0) return nullptr;
    return new int[n];
}

void init_mas(int* mas, int n) {
    for (int i = 0; i < n; i++) mas[i] = rand() % 100 - 50;
}

void print_mas(int* mas, int n) {
    if (!mas || n <= 0) {
        cout << "Масив порожній!" << endl;
        return;
    }
    for (int i = 0; i < n; i++) cout << mas[i] << " ";
    cout << endl;
}

// Додаємо після кожного негативного елемента модуль
int* process_1d(int*& mas, int& n) {
    int count_neg = 0;
    for (int i = 0; i < n; i++) if (mas[i] < 0) count_neg++;

    int new_n = n + count_neg;
    int* new_mas = new int[new_n];

    for (int i = 0, j = 0; i < n; i++) {
        new_mas[j++] = mas[i];
        if (mas[i] < 0) {
            new_mas[j++] = abs(mas[i]);
        }
    }

    delete[] mas;
    mas = new_mas;
    n = new_n;
    return mas;
}

int** form_2d(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return nullptr;
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) matrix[i] = new int[cols];
    return matrix;
}

void init_2d(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = rand() % 100 - 50;
}

void print_2d(int** matrix, int rows, int cols) {
    if (!matrix || rows <= 0 || cols <= 0) {
        cout << "Матриця порожня!" << endl;
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) cout << setw(5) << matrix[i][j];
        cout << endl;
    }
}

void delete_2d(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) delete[] matrix[i];
    delete[] matrix;
}

// Видаляємо парні стовпці
int** process_2d(int**& matrix, int rows, int& cols) {
    if (!matrix || cols <= 0) {
        cout << "Помилка: спроба видалення з порожньої матриці!" << endl;
        return matrix;
    }

    int new_cols = cols - (cols / 2);
    if (new_cols == cols) return matrix;

    int** new_matrix = form_2d(rows, new_cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0, new_j = 0; j < cols; j++) {
            if (j % 2 == 0) { 
                new_matrix[i][new_j++] = matrix[i][j];
            }
        }
    }

    delete_2d(matrix, rows);
    matrix = new_matrix;
    cols = new_cols;
    return matrix;
}

int main() {
    setlocale(LC_ALL, "Ukrainian");
    srand(time(0));

    int* mas = nullptr;
    int n = 0, rows = 0, cols = 0;
    int** matrix = nullptr;
    int choice;

    do {
        cout << "\nЩо робимо?" << endl;
        cout << "1. Створити масив" << endl;
        cout << "2. Додати модуль після негативних чисел в масиві" << endl;
        cout << "3. Створити матрицю" << endl;
        cout << "4. Bидалити парні стовпці в матиці" << endl;
        cout << "5. Закіинчити роботу" << endl;
        cout << "Твій вибір -> "; cin >> choice;

        switch (choice) {
            case 1:
                if (mas) delete[] mas;
                cout << "Розмір n -> "; cin >> n;
                mas = form_mas(n);
                init_mas(mas, n);
                print_mas(mas, n);
                break;
            case 2:
                mas = process_1d(mas, n);
                print_mas(mas, n);
                break;
            case 3:
                if (matrix) delete_2d(matrix, rows);
                cout << "Рядки та стовпці (i j) -> "; cin >> rows >> cols;
                matrix = form_2d(rows, cols);
                init_2d(matrix, rows, cols);
                print_2d(matrix, rows, cols);
                break;
            case 4:
                matrix = process_2d(matrix, rows, cols);
                print_2d(matrix, rows, cols);
                break;
        }
    } while (choice != 5);

    if (mas) delete[] mas;
    if (matrix) delete_2d(matrix, rows);

    return 0;
}