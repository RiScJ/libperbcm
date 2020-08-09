#include <iostream>

#include "gpio.hpp"

using namespace std;

const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string yellow("\033[0;33m");
const std::string blue("\033[0;34m");
const std::string magenta("\033[0;35m");
const std::string cyan("\033[0;36m");

const std::string reset("\033[0m");

void TEST_GPIO(void) {
	while (true) {
		for (int pin = 0; pin < 20; pin++) {
			bool state = GPIO(pin).read();
			string code = state ? green : red;
			cout << code << state << reset << " ";
		}
		cout << "\r";
	}
};

int main()
{
	TEST_GPIO();
	return 0;
}
