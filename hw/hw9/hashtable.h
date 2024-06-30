#ifndef hashtable_h_
#define hashtable_h_

#include<iostream>
#include<string>
#include<vector>
#include"tableentry.h"

class Hashtable {
public:
	//constructor
	Hashtable(int s, float o, int k) { table = std::vector<TableEntry>(s); occupancy = o; k_size = k; size = 0; }

	//public member functions
	int get_k() const { return k_size; }
	void check_size();
	void insert(const std::string& kmer, int loc);
	std::vector<int> find(const std::string& kmer) const;

private:
	//member variables
	std::vector<TableEntry> table;
	int k_size;
	int size;
	float occupancy;

	//private member functions
	int hash(const std::string& kmer) const;
};

#endif