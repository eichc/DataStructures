#include<map>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[]) {
	std::ifstream in_str(argv[1]);

	//read values and at the same time print them in the order they are read
	std::map<int, int> nums;
	int num;
	while (in_str >> num) {
		std::cout << num << ' ';
		++nums[num];
	}
	std::cout << std::endl;

	//find mode
	int mode = 0;
	std::map<int, int>::iterator itr;
	for (itr = nums.begin(); itr != nums.end(); ++itr) {
		if (itr->second > mode) {
			mode = itr->second;
		}
	}

	//print results
	std::cout << "Mode(s): ";
	for (itr = nums.begin(); itr != nums.end(); ++itr) {
		if (itr->second == mode) {
			std::cout << itr->first << ' ';
		}
	}
	std::cout << std::endl;


	return 0;
}