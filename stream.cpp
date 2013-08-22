#include <iostream>

#include "stream.hpp"

using namespace std;

int main() {
	auto s = Stream<int>::range(10, 100)
				->take(20)
				->filter([](int a) { return a % 2 == 0; })
				->map<float>([](int a) { return a + 0.5f; });
	while(!s->empty()) {
		cout << *s->head() << endl;
		s = s->tail();
	}
	return 0;
}
