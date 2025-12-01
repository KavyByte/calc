// fcfs.cpp
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n, head;
    cout << "Enter number of requests: ";
    cin >> n;

    int req[n];
    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++) cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    int total = 0;

    for (int i = 0; i < n; i++) {
        total += abs(req[i] - head);
        head = req[i];
    }

    cout << "Total head movement = " << total << endl;
    return 0;
}
