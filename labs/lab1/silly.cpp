#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
	int product = 1;
	for (int i = 1; i < argc; i++) {
		int num = std::stoi(argv[i]);
		product *= num;
	}

	std::cout << "product of integers: " << product << std::endl;
}