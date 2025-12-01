//READER WRITER
// readers_writers.cpp
// Compile: g++ -std=c++17 readers_writers.cpp -pthread -o readers_writers
// Run: ./readers_writers <num_readers> <num_writers> <rounds>
// Example: ./readers_writers 3 2 5

#include <bits/stdc++.h>
using namespace std;

int data_store = 0;
mutex m;
condition_variable cv;
int active_readers = 0;
bool writer_active = false;

void reader(int id, int rounds) {
    for (int i=0;i<rounds;++i){
        unique_lock<mutex> lk(m);
        cv.wait(lk, [](){ return !writer_active; }); // wait while writer active
        ++active_readers;
        lk.unlock();

        // reading
        cout << "[R" << id << "] read value = " << data_store << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        lk.lock();
        --active_readers;
        if (active_readers==0) cv.notify_all();
        lk.unlock();

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void writer(int id, int rounds) {
    for (int i=0;i<rounds;++i){
        unique_lock<mutex> lk(m);
        cv.wait(lk, [](){ return !writer_active && active_readers==0; });
        writer_active = true;
        lk.unlock();

        // writing
        ++data_store;
        cout << "  [W" << id << "] wrote value = " << data_store << "\n";
        this_thread::sleep_for(chrono::milliseconds(150));

        lk.lock();
        writer_active = false;
        lk.unlock();
        cv.notify_all();

        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main(int argc, char** argv){
    if (argc < 4) { cout << "Usage: " << argv[0] << " <num_readers> <num_writers> <rounds>\n"; return 1; }
    int nr = stoi(argv[1]), nw = stoi(argv[2]), rounds = stoi(argv[3]);
    vector<thread> readers, writers;
    for (int i=0;i<nr;++i) readers.emplace_back(reader, i+1, rounds);
    for (int i=0;i<nw;++i) writers.emplace_back(writer, i+1, rounds);

    for (auto &t: readers) t.join();
    for (auto &t: writers) t.join();
    cout << "Finished.\n";
}
