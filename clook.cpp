// clook.cpp
#include <iostream>
#include <algorithm>
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

    sort(req, req + n);

    int total = 0;
    int pos = 0;

    while (pos < n && req[pos] < head) pos++;

    // Move right first
    for (int i = pos; i < n; i++) {
        total += abs(req[i] - head);
        head = req[i];
    }

    // Jump to lowest request
    total += abs(head - req[0]);
    head = req[0];

    // Continue from start
    for (int i = 0; i < pos; i++) {
        total += abs(req[i] - head);
        head = req[i];
    }

    cout << "Total head movement = " << total << endl;
    return 0;
}
