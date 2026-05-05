// Завдання 2.5.2

#include <iostream>
#include <string>
using namespace std;

const int SIZE = 11;

string encrypt(string text) {
    char matrix[SIZE][SIZE];
    int k = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (k < text.length()) 
                matrix[i][j] = text[k++];
            else 
                matrix[i][j] = ' '; 
        }
    }

    string result = "";
    int r = 5, c = 5; 
    
    result += matrix[r][c];
    int step = 1;

    while (step < SIZE) {
        // вправо
        for (int i = 0; i < step; i++) if (++c < SIZE) result += matrix[r][c];
        // вниз
        for (int i = 0; i < step; i++) if (++r < SIZE) result += matrix[r][c];
        step++;
        // вліво
        for (int i = 0; i < step; i++) if (--c >= 0) result += matrix[r][c];
        // вгору
        for (int i = 0; i < step; i++) if (--r >= 0) result += matrix[r][c];
        step++;
    }

    for (int i = 0; i < step - 1; i++) if (++c < SIZE) result += matrix[r][c];
    
    return result;
}

string decrypt(string encryptedText) {
    char matrix[SIZE][SIZE];

    int k = 0;
    int r = 5, c = 5;
    
    if (k < encryptedText.length()) matrix[r][c] = encryptedText[k++];

    int step = 1;
    
    // Так само як і при шифруванні 
    while (step < SIZE) {
        for (int i = 0; i < step; i++) 
            if (++c < SIZE && k < encryptedText.length()) matrix[r][c] = encryptedText[k++];
        for (int i = 0; i < step; i++) 
            if (++r < SIZE && k < encryptedText.length()) matrix[r][c] = encryptedText[k++];
        step++;
        for (int i = 0; i < step; i++) 
            if (--c >= 0 && k < encryptedText.length()) matrix[r][c] = encryptedText[k++];
        for (int i = 0; i < step; i++) 
            if (--r >= 0 && k < encryptedText.length()) matrix[r][c] = encryptedText[k++];
        step++;
    }
    for (int i = 0; i < step - 1; i++) 
        if (++c < SIZE && k < encryptedText.length()) matrix[r][c] = encryptedText[k++];


    // Зберігаємо тут
    string result = "";

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            result += matrix[i][j];
            
    return result;
}

int main() {
    setlocale(LC_ALL, "Ukrainian");

    string text;
    cout << "Текст для шифрування -> ";
    getline(cin, text);

    string encrypted = encrypt(text);
    cout << "\nЗашифрований текст ->\n" << encrypted << endl;

    string decrypted = decrypt(encrypted);
    cout << "\nРозшифрований текст ->\n" << decrypted << endl;

    return 0;
}