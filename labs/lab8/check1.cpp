#include<iostream>


int count_paths(int x, int y) {
	if (x == 0 && y == 0) {
		return 1;
	}

	int paths = 0;
	if (x-1 >= 0) {
		paths += count_paths(x-1, y);
	}
	if (y-1 >= 0) {
		paths += count_paths(x, y-1);
	}
	return paths;
}

int main() {
	int x, y;
	while (true) {
		std::cout << "Enter starting x and y values, separated by a space (Type -1 to exit):";
		std::cin >> x;
		if (x == -1) {
			break;
		}
		std::cin >> y;

		std::cout << "Total paths from (" << x << ", " << y << "): " << count_paths(x,y) << std::endl;
	}

	return 0;
}