#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id, bt, rt, at, ct, tat, wt;
};

int main() {
    int n;
    cin >> n;
    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i].at >> p[i].bt;
        p[i].id = i;
        p[i].rt = p[i].bt;
    }

    queue<int> q1, q2, q3;
    int tq1 = 4, tq2 = 8;
    int time = 0, done = 0;
    vector<bool> pushed(n, false);

    while (done < n) {
        for (int i = 0; i < n; i++)
            if (!pushed[i] && p[i].at <= time) { q1.push(i); pushed[i] = true; }

        if (!q1.empty()) {
            int i = q1.front(); q1.pop();
            int t = min(tq1, p[i].rt);
            time += t;
            p[i].rt -= t;
            if (p[i].rt == 0) { p[i].ct = time; done++; }
            else q2.push(i);
        }
        else if (!q2.empty()) {
            int i = q2.front(); q2.pop();
            int t = min(tq2, p[i].rt);
            time += t;
            p[i].rt -= t;
            if (p[i].rt == 0) { p[i].ct = time; done++; }
            else q3.push(i);
        }
        else if (!q3.empty()) {
            int i = q3.front(); q3.pop();
            time += p[i].rt;
            p[i].rt = 0;
            p[i].ct = time;
            done++;
        }
        else time++;
    }

    cout << "ID  AT  BT  CT  TAT  WT\n";
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        cout << "P" << p[i].id << "  " << p[i].at << "   " << p[i].bt << "   "
             << p[i].ct << "   " << p[i].tat << "   " << p[i].wt << "\n";
    }
}
