#include <iostream>
using namespace std;

int main () {
  int n, k1, k2;
  
  cout << "Number of elemnts for n -> ";
  cin >> n;
  
  cout << "Enter k1: ";
  cin >> k1;
  
  cout << "Enter k2: ";
  cin >> k2;
  
  int coutFor = 0;
  cout << "Enter " << n << "prime numbers: " << endl;
  
  for (int i = 1; i <= n; i++) {
    int num;
    
    cout << "Element " << i << ": ";
    cin >> num;
    
    if (num % k1 == 0 && num % k2 != 0) {
      coutFor++;
    }

  }
  
  cout << "Result: " << coutFor << endl;
  return 0;
}