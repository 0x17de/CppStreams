#include <iostream>

#include "stream.hpp"

using namespace std;

int main()
{
    Stream<int>::range(5)
    .filter([] (int num)
    {
        return num % 2 == 0;
    })
    .take(5)
    .map<float>([] (int num)
    {
         return (float)num + 0.5f;
    })
    .walk([] (float num)
    {
        cout << num << endl;
    });
}
