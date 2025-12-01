// DINING PHILOSOPHER
// dining_philosophers.cpp
// Compile: g++ -std=c++17 dining_philosophers.cpp -pthread -o dining_philosophers
// Run: ./dining_philosophers <num_philosophers> <rounds>
// Example: ./dining_philosophers 5 3

#include <bits/stdc++.h>
using namespace std;

int N = 5;
int rounds = 3;
vector<mutex> forks;
mutex waiter_m;
int allowed; // how many philosophers can try to pick forks concurrently

void philosopher(int id) {
    for (int r=0;r<rounds;++r){
        // thinking
        cout << "[P" << id << "] thinking\n";
        this_thread::sleep_for(chrono::milliseconds(100 + rand()%100));

        // request permission from waiter (at most N-1 can try)
        {
            unique_lock<mutex> lk(waiter_m);
            while (allowed == 0) {
                // spin-wait style but using condition would be nicer; keep simple
                lk.unlock();
                this_thread::sleep_for(chrono::milliseconds(10));
                lk.lock();
            }
            --allowed;
        }

        // pick up forks (left then right)
        int left = id;
        int right = (id+1)%N;
        // to avoid deadlock further pick lower-numbered fork first
        int f1 = min(left,right), f2 = max(left,right);
        forks[f1].lock();
        forks[f2].lock();

        // eat
        cout << "[P" << id << "] eating (round " << r+1 << ")\n";
        this_thread::sleep_for(chrono::milliseconds(120 + rand()%120));

        // put down forks
        forks[f2].unlock();
        forks[f1].unlock();

        // release waiter permission
        {
            lock_guard<mutex> lk(waiter_m);
            ++allowed;
        }

        // back to thinking
    }
    cout << "[P" << id << "] done.\n";
}

int main(int argc, char** argv){
    if (argc>=2) N = stoi(argv[1]);
    if (argc>=3) rounds = stoi(argv[2]);
    forks.resize(N);
    allowed = N-1;

    vector<thread> ps;
    for (int i=0;i<N;++i) ps.emplace_back(philosopher, i);
    for (auto &t: ps) t.join();
    cout << "All philosophers finished.\n";
}
