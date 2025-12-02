#include <bits/stdc++.h>
using namespace std;

int extended_euclidean_algo(int a, int b) {
  if (a < b)
    return extended_euclidean_algo(b, a);

  int q;
  int r1 = a, r2 = b, r;
  int s1 = 1, s2 = 0, s;
  int t1 = 0, t2 = 1, t;

  // Table header
  cout << left << setw(5) << "q" << setw(8) << "r1" << setw(8) << "r2"
       << setw(8) << "r" << setw(8) << "s1" << setw(8) << "s2" << setw(8) << "s"
       << setw(8) << "t1" << setw(8) << "t2" << setw(8) << "t" << endl;

  cout << string(70, '-') << endl; // Separator line

  while (r2 != 0) {
    q = r1 / r2;
    r = r1 % r2;

    s = s1 - s2 * q;
    t = t1 - t2 * q;

    cout << left << setw(5) << q << setw(8) << r1 << setw(8) << r2 << setw(8)
         << r << setw(8) << s1 << setw(8) << s2 << setw(8) << s << setw(8) << t1
         << setw(8) << t2 << setw(8) << t << endl;

    // updating values
    r1 = r2;
    r2 = r;
    s1 = s2;
    s2 = s;
    t1 = t2;
    t2 = t;
  }

  return r1;
}

int main() {
  int gcd = extended_euclidean_algo(161, 28);
  cout << "\nGCD : " << gcd << "\n\n";
  gcd = extended_euclidean_algo(240, 46);
  cout << "\nGCD : " << gcd << "\n";
  return 0;
}
