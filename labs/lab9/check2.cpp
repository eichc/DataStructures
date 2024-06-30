#include<map>
#include<iostream>
#include<fstream>

int main(int argc, char* argv[]) {
	std::ifstream in_str(argv[1]);

	//read and print values while also finding the mode
	std::map<int, int> nums;
	int num;
	int mode = 0;
	std::map<int, int>::iterator search;
	while (in_str >> num) {
		std::cout << num << ' ';
		if (nums.find(num) == nums.end()) {
			search = nums.insert(std::make_pair(num, 0)).first;
		} else {
			search = nums.find(num);
		}
		++search->second;
		//update mode
		if (search->second > mode) {
			mode = search->second;
		}
	}
	std::cout << std::endl;

	//print results
	std::cout << "Mode(s): ";
	std::map<int,int>::iterator itr;
	for (itr = nums.begin(); itr != nums.end(); ++itr) {
		if (itr->second == mode) {
			std::cout << itr->first << ' ';
		}
	}
	std::cout << std::endl;


	return 0;
}