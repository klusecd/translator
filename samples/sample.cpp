#include "translator.h"

int main() {
	std::cout << "Enter arithmetic expression (example: 6+7*(5-2))" << std::endl;
	std::string s;
	std::cin >> s;
	Arithmetic_expressions a(s);
	double rez = a.solve();
	std::cout << rez << std::endl;
	return 0;
}