// Завдання 2.5.1

#include <iostream>
#include <string>
using namespace std;

int countWords(string str) {
    int count = 0;
    bool isWord = false;

    for (int i = 0; i < str.length(); i++) {

        if (str[i] != ' ' && !isWord) {
            isWord = true;
            count++;
        } 

        else if (str[i] == ' ') {
            isWord = false;
        }
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "Ukrainian");
    string input;
    
    cout << "Введіть рядок тут -> ";
    getline(cin, input); 

    cout << "Кількість слів -> " << countWords(input) << endl;

    return 0;
}