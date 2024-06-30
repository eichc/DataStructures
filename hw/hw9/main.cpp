#include<iostream>
#include<fstream>
#include<string>
#include"hashtable.h"

//fill the hashtable using the data from the genome file
void fill_table(Hashtable& table, const std::string& genome) {
	for (unsigned int i = 0; i < genome.size()-table.get_k()+1; ++i) {
		table.insert(genome.substr(i, table.get_k()), i);
	}
}

//Given a string, print where it occurs in the genome with no more than max_m mismatches
void handle_query(const Hashtable& table, const std::string& genome, const std::string& query, int max_m) {
	std::cout << "Query: " << query << std::endl;
	std::string kmer = query.substr(0, table.get_k());
	std::vector<int> locs = table.find(kmer);

	if (locs.size() == 0) {
		std::cout << "No Match" << std::endl;
		return;
	}
	//at each location where the seed is found, look for the rest of the query
	int matches = 0;
	for (unsigned int i = 0; i < locs.size(); ++i) {
		//if the rest of the genome is shorter than the query, it is impossible for there to be a match
		std::string sub = genome.substr(locs[i], query.size());
		if (query.size() > sub.size()) {
			continue;
		}

		//add one letter at a time to avoid going over max_m mismatches
		int mismatches = 0;
		unsigned int j = 0;
		while (j < sub.size() && mismatches <= max_m) {
			if (sub[j] != query[j]) {
				mismatches++;
			}
			++j;
		}
		if (mismatches <= max_m) {
			matches++;
			std::cout << locs[i] << ' ' << mismatches << ' ' << sub << std::endl;
		}
	}
	
	if (matches == 0) {
		std::cout << "No Match" << std::endl;
	}
}

int main() {
	//set up genome input file
	std::string action, value;
	std::cin >> action >> value;
	std::ifstream g_file(value);

	//create the initial hashtable
	int init_size = 100;
	float occupancy = 0.5;
	int kmer;
	while (true) {
		std::cin >> action >> value;
		if (action == "table_size") {
			init_size = std::stoi(value);
		}
		if (action == "occupancy") {
			occupancy = std::stof(value);
		}
		if (action == "kmer") {
			kmer = std::stoi(value);
			break;
		}
	}
	Hashtable table(init_size, occupancy, kmer);

	//fill the hashtable
	std::string genome, temp_str;
	while (g_file >> temp_str) {
		genome += temp_str;
	}
	fill_table(table, genome);

	//go through each query
	std::string mismatches, query;
	while (std::cin >> action) {
		if (action == "quit") {
			break;
		}
		std::cin >> mismatches >> query;
		handle_query(table, genome, query, std::stoi(mismatches));
	}

	return 0;
}