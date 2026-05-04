#include <iostream>
#include <cmath>
using namespace std;

int main() {

  double c, z, x, result;
  
  cout << "Enter c -> ";
  cin >> c;
  
  z = cos(c);
  cout << "z = cos(" << c << ") = " << z << endl;
  
  if (z < 0) {
    // f(z), if z < 0
    double z15 = pow(fabs(z), 1.0 / 5.0) * (z < 0 ? -1 : 1);

    x = z / sin(z15);
    cout << "f1(z)" << endl;

  } else if (z >= 0 && z <= 8) {
    // f(z), if 0 <= z <= 8
    double tanz = tan(z);

    x = 2 * z * tanz * tanz * tanz;
    cout << "f2(z)" << endl;

  } else {
    // f(z), if z > 8
    x = z * sqrt(fabs(z)) + 8;
    cout << "f3(z)" << endl;
  }
  
  cout << "x = " << x << endl;
  
  if (x == 0) {
    cout << "Error! Can't divide by zero" << endl;
    return 1;
  }
 
  // Calculate f(x); print out the result
  result = tan(4 * x) + 1.0 / x;
  cout << "f(x) = " << result << endl;
  
  return 0;  
}