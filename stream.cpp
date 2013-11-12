#include <iostream>

#include "stream.hpp"

using namespace std;

int main()
{
    Stream<int>::range(5) // build a stream from 5 to infinity
    .filter([] (int num) // only accept even numbers
    {
        return num % 2 == 0;
    })
    .take(5) // only take 5 even numbers
    .map<float>([] (int num) // add 0.5 to each number and convert the stream to a float stream
    {
         return (float)num + 0.5f;
    })
    .walk([] (float num) // list the results
    {
        cout << num << endl;
    });
}
