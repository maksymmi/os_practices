#include <iostream>
#include <iomanip> 
#include <cmath>   

using namespace std;

int main() {

    double a, b, h;

    cout << "Enter a: ";
    cin >> a;
    cout << "Enter b: ";
    cin >> b;
    cout << "Enter h: ";
    cin >> h;

    cout << "-----------------------" << endl;
    cout << setw(10) << "x" << " | " << setw(10) << "F(x)" << endl;
    cout << "-----------------------" << endl;

    cout << fixed << setprecision(4);

    for (double x = a; x <= b + h / 2; x += h) {
        // Обчислення ctg(x/2) як 1/tan(x/2)
        // Функція: F(x) = ctg(x/2) + 1 / (2 * sin(x))
        double f_x = (1.0 / tan(x / 2.0)) + (1.0 / (2.0 * sin(x)));

        cout << setw(10) << x << " | " << setw(10) << f_x << endl;
    }

    cout << "-----------------------" << endl;

    return 0;
}