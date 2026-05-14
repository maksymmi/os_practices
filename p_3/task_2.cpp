#include <iostream>
#include <iomanip> 
#include <cstdlib> 
#include <ctime>  

using namespace std;

const int MAX_SIZE = 100;

void fillArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 101 - 50; 
    }
}

void printArray(int arr[], int n) {
    cout << "Масив: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

bool isFibonacci(int n) {

    if (n < 0) return false; 
    if (n == 0 || n == 1) return true;

    int a = 0, b = 1;

    while (b < n) {
        int next = a + b;
        a = b;
        b = next;
    }

    return (b == n);
}

void processArray(int arr[], int n) {

    int count = 0;

    for (int i = 0; i < n; i++) {
        if (isFibonacci(arr[i])) {
            count++;
        }
    }

    cout << "Кількість чисел Фібоначчі в масиві: " << count << endl;
}

void fillMatrix(int mat[MAX_SIZE][MAX_SIZE], int rows, int cols) {

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {
            mat[i][j] = rand() % 101 - 50;
        }
    }
}

void printMatrix(int mat[MAX_SIZE][MAX_SIZE], int rows, int cols) {

    cout << "Матриця:" << endl;

    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < cols; j++) {
            cout << setw(5) << mat[i][j];
        }

        cout << endl;
    }
}

void sortColumns(int mat[MAX_SIZE][MAX_SIZE], int rows, int cols) {

    for (int j = 0; j < cols; j++) { 

        for (int step = 0; step < rows - 1; step++) {

            for (int i = 0; i < rows - step - 1; i++) {

                if (mat[i][j] < mat[i + 1][j]) { 

                    int temp = mat[i][j];

                    mat[i][j] = mat[i + 1][j];
                    mat[i + 1][j] = temp;
                }
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Ukrainian");
    srand(time(0)); 

    // 1. одновимірний масив
    int n;
    int arr[MAX_SIZE];
    cout << "Розмір одновимірного масиву -> ";
    cin >> n;

    fillArray(arr, n);
    printArray(arr, n);
    processArray(arr, n);

    cout << "\n" << string(30, '-') << "\n\n";

    // 2. матриця
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    cout << "Kількість рядків матриці -> ";
    cin >> rows;
    cout << "Kількість стовпців -> ";
    cin >> cols;

    fillMatrix(matrix, rows, cols);
    cout << "До сортування ->" << endl;
    printMatrix(matrix, rows, cols);

    sortColumns(matrix, rows, cols);

    cout << "\nПісля сортування ->" << endl;
    printMatrix(matrix, rows, cols);

    return 0;
}