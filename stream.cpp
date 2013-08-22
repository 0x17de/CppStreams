#include <iostream>

#include "stream.hpp"

using namespace std;

int main() {
	auto s = Stream<int>::range(10, 30)->take(5);
	while(!s->empty()) {
		cout << *s->head() << endl;
		s = s->tail();
	}
	return 0;
}
