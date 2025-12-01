// PRODUCER CONSUMER
// producer_consumer.cpp
// Compile: g++ -std=c++17 producer_consumer.cpp -pthread -o producer_consumer
// Run: ./producer_consumer <buffer_size> <num_items> <num_consumers>
// Example: ./producer_consumer 5 30 2

#include <bits/stdc++.h>
using namespace std;

deque<int> buffer;
mutex m;
condition_variable cv_full, cv_empty;
int capacity = 5;
atomic<int> produced_count{0};

void producer(int total) {
    int next = 0;
    while (next < total) {
        unique_lock<mutex> lk(m);
        cv_full.wait(lk, [](){ return (int)buffer.size() < capacity; });
        buffer.push_back(next);
        cout << "[P] produced " << next << "\n";
        ++next;
        ++produced_count;
        lk.unlock();
        cv_empty.notify_one();
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void consumer(int id, int total_produced) {
    while (true) {
        unique_lock<mutex> lk(m);
        cv_empty.wait(lk, [&](){ return !buffer.empty() || produced_count >= total_produced; });
        if (buffer.empty() && produced_count >= total_produced) break;
        int item = buffer.front();
        buffer.pop_front();
        cout << "  [C" << id << "] consumed " << item << "\n";
        lk.unlock();
        cv_full.notify_one();
        this_thread::sleep_for(chrono::milliseconds(80 + id*20));
    }
}

int main(int argc, char** argv){
    if (argc < 4) { cout << "Usage: " << argv[0] << " <buffer_size> <num_items> <num_consumers>\n"; return 1; }
    capacity = stoi(argv[1]);
    int total = stoi(argv[2]);
    int consumers = stoi(argv[3]);

    thread prod(producer, total);
    vector<thread> cons;
    for (int i=0;i<consumers;++i) cons.emplace_back(consumer, i+1, total);

    prod.join();
    // wake consumers possibly waiting
    {
        lock_guard<mutex> lk(m);
    }
    cv_empty.notify_all();
    for (auto &t: cons) t.join();
    cout << "Done.\n";
}
