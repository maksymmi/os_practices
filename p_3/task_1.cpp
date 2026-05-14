#include <iostream>
#include <cmath> 
using namespace std;

int main() {
    setlocale(LC_ALL, "Ukrainian");

    double s, t;
    
    cout << "Pначення s -> ";
    cin >> s;

    cout << "Значення t -> ";
    cin >> t;

    // накопичення значень p
    double p1 = 0, pt = 0, pst = 0;

    cout << "Вводьте коефіцієнти a0, a1 до a12 ->" << endl;

    for (int i = 0; i <= 12; i++) {
        double a;
        cout << "a[" << i << "] = ";
        cin >> a;

        p1 += a * pow(1, i); 
        
        // p(t)
        pt += a * pow(t, i);
        
        // p(s-t)
        pst += a * pow(s - t, i);
    }

    // p(1) - p(t) + p^2(s-t) - p^3(1)
    double result = p1 - pt + pow(pst, 2) - pow(p1, 3);

    cout << "\nРезультат -> " << result << endl;

    return 0;
}