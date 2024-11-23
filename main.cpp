#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

mutex mut;

condition_variable cv;

vector<int> numbers;

bool readyForNextNumber = false;

void provide(int number)
{
    unique_lock<mutex> ul(mut);

    numbers.push_back(number);
    cout << "thread: " << this_thread::get_id() << " provided a number: " << number << " at list" << endl;

    readyForNextNumber = true;
    cv.notify_one();

    this_thread::sleep_for(chrono::seconds(1));
}

void consume()
{
    unique_lock<mutex> ul(mut);

    cv.wait(ul, [] { return readyForNextNumber; });

    readyForNextNumber = false;

    cout << "thread: " << this_thread::get_id() << " consumed a number: " << numbers.back() << " from list\n"<< endl;

}

int main()
{
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        thread provider(provide, rand() % 1000);
        thread consumer(consume);
        provider.join();
        consumer.join();
    }
}
