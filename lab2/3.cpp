#include <iostream>
using namespace std;

// Null Pointer Dereferencing
int main() {
  int *ptr = nullptr;
  cout << "Enter a number: ";
  cin >> *ptr;
  cout << "Value: " << *ptr << endl;
  return 0;
}
