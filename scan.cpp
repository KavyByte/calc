// scan.cpp
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    int n, head, disk;
    cout << "Enter number of requests: ";
    cin >> n;

    int req[n];
    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++) cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter total disk size: ";
    cin >> disk;

    int direction;
    cout << "Direction (0 = left, 1 = right): ";
    cin >> direction;

    sort(req, req + n);

    int total = 0;
    int pos = 0;

    while (pos < n && req[pos] < head) pos++;

    if (direction == 1) {
        // Right
        for (int i = pos; i < n; i++) {
            total += abs(req[i] - head);
            head = req[i];
        }
        // Move to end
        total += abs((disk - 1) - head);
        head = disk - 1;

        // Come back left
        for (int i = pos - 1; i >= 0; i--) {
            total += abs(req[i] - head);
            head = req[i];
        }
    } else {
        // Left
        for (int i = pos - 1; i >= 0; i--) {
            total += abs(req[i] - head);
            head = req[i];
        }
        // Move to start
        total += abs(head - 0);
        head = 0;

        // Move right
        for (int i = pos; i < n; i++) {
            total += abs(req[i] - head);
            head = req[i];
        }
    }

    cout << "Total head movement = " << total << endl;
    return 0;
}
