#include <bits/stdc++.h>
using namespace std;

// Buffer Overflow
int main() {
  char buffer[10];
  cout << "Enter your name: ";
  cin >> buffer;
  cout << "Hello, " << buffer << "!" << endl;
  return 0;
}

// int main() {
//   char arr[2];
//   arr[0] = 'a';
//   arr[1] = 'b';
//   arr[1] += 127;
//   cout << arr[0] << "\n";
//   cout << (int)(arr[1]) << "\n";
//   return 0;
// }
