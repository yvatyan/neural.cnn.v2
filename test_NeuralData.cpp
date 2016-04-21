#include <iostream>

#include "Sources_neural\Headers\MultiDdata.hpp"

using namespace std;

int main() {
	vector<double>* test = new vector<double>(20, 5);
	for(int i = 0; i < test->size(); ++i) {
		cout << test->operator[](i) << ' ';
	}
	return 0;
}
