#include <iostream>
using namespace std;

int main() {

  long long n;
  int lastDigit, lastDigitCube;
  
  cout << "Enter your number -> ";
  cin >> n;
  
  lastDigit = abs(n % 10);
  
  switch (lastDigit) {
    case 0: lastDigitCube = 0; break;  // 0^3 = 0
    case 1: lastDigitCube = 1; break;  // 1^3 = 1
    case 2: lastDigitCube = 8; break;  // 2^3 = 8
    case 3: lastDigitCube = 7; break;  // 3^3 = 27
    case 4: lastDigitCube = 4; break;  // 4^3 = 64
    case 5: lastDigitCube = 5; break;  // 5^3 = 125
    case 6: lastDigitCube = 6; break;  // 6^3 = 216
    case 7: lastDigitCube = 3; break;  // 7^3 = 343
    case 8: lastDigitCube = 2; break;  // 8^3 = 512
    case 9: lastDigitCube = 9; break;  // 9^3 = 729
  }
  
  cout << "Number: " << n << endl;
  cout << "Last digit -> " << lastDigit << endl;
  cout << "Last digit of cube -> " << lastDigitCube << endl;
  
  return 0;
}