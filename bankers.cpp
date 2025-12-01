#include <iostream>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    int alloc[n][m], maxNeed[n][m], avail[m], need[n][m], finish[n]={0}, ans[n], ind=0;

    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin >> alloc[i][j];

    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin >> maxNeed[i][j];

    for(int i=0;i<m;i++)
        cin >> avail[i];

    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            need[i][j] = maxNeed[i][j] - alloc[i][j];

    while(ind < n) {
        bool safe = false;
        for(int i=0;i<n;i++) {
            if(finish[i] == 0) {
                int j;
                for(j=0;j<m;j++)
                    if(need[i][j] > avail[j]) break;
                if(j == m) {
                    for(int k=0;k<m;k++)
                        avail[k] += alloc[i][k];
                    finish[i] = 1;
                    ans[ind++] = i;
                    safe = true;
                }
            }
        }
        if(!safe) break;
    }

    if(ind < n) cout << "System is not safe";
    else {
        cout << "Safe Sequence: ";
        for(int i=0;i<n;i++)
            cout << "P" << ans[i] << (i == n-1 ? "" : " -> ");
    }
}
