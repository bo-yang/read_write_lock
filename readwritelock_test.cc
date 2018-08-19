#include <iostream>
#include <thread>
#include <list>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "readwritelock.hh"

ReadWriteLock rwlck;
const size_t buffer_size = 20;
std::list<int> cargo;
const int max_cargo = 100;
int total_cargo = 0;

void consumer() {
    bool iamdone = false;
    while (true) {
        rwlck.write_lock();
        if (cargo.size()) {
            std::cout << cargo.front() << " consumed by " << std::this_thread::get_id() << std::endl;
            cargo.pop_front();
        }
        iamdone = cargo.empty() && (total_cargo >= max_cargo);
        rwlck.write_unlock();

        if (iamdone)
            break;

        std::this_thread::sleep_for (std::chrono::milliseconds(rand()%15+1));
    }
}

void producer(int start, int cnt) {
    int i = start;
    while (i < start+cnt) {
        rwlck.write_lock();
        if (cargo.size() < buffer_size) {
            std::cout << i << " produced by " << std::this_thread::get_id() << std::endl;
            cargo.push_back(i++);
            total_cargo++;
        }
        rwlck.write_unlock();
        std::this_thread::sleep_for (std::chrono::milliseconds(rand()%15+1));
    }
}

void watcher() {
    bool iamdone = false;
    while (true) {
        rwlck.read_lock();
        if (total_cargo % 10 == 0) {
            std::cout << std::this_thread::get_id() << ": total_cargo = " << total_cargo << std::endl;
            for (auto c : cargo)
                std::cout << c << " ";
            std::cout << std::endl;
        }
        iamdone = (total_cargo >= max_cargo);
        rwlck.read_unlock();

        if (iamdone)
            break;

        std::this_thread::sleep_for (std::chrono::milliseconds(rand()%8+1));
    }
}

int main()
{
    srand(time(NULL));
    const int prod_threads = 5;
    const int cons_threads = 3;
    const int watch_threads = 2;
    std::vector<std::thread> consumers, producers, watchers;
    for (int i = 0; i < prod_threads; ++i)
        producers.push_back(std::thread(producer, i*max_cargo/prod_threads, max_cargo/prod_threads));
    for (int i = 0; i < cons_threads; ++i)
        consumers.push_back(std::thread(consumer));
    for (int i = 0; i < watch_threads; ++i)
        watchers.push_back(std::thread(watcher));

    for (int i = 0; i < prod_threads; ++i)
        producers[i].join();
    for (int i = 0; i < cons_threads; ++i)
        consumers[i].join();
    for (int i = 0; i < watch_threads; ++i)
        watchers[i].join();
}
