#include"hashtable.h"

//check to see if the occupancy has exceeded the maximum, and if so rehash the data.
void Hashtable::check_size() {
	float curr_occ = float(size) / table.size();
	if (curr_occ <= occupancy) {
		return;
	}
	std::vector<TableEntry> new_table(table.size()*2);
	//copy the entries into the new table
	for (unsigned int i = 0; i < table.size(); ++i) {
		if (table[i].key != "") {
			int hash_val = hash(table[i].key) % new_table.size();
			while (new_table[hash_val].key != "") {
				hash_val++;
				hash_val %= new_table.size();
			}
			new_table[hash_val].key = table[i].key;
			new_table[hash_val].locations = table[i].locations;
		}
	}

	table = new_table;
}

//return an integer that represents where the kmer belongs in the table.
int Hashtable::hash(const std::string& kmer) const {
	int ret = 0;
	for (unsigned int i = 0; i < kmer.size(); ++i) {
		ret += (kmer[i] * (i+1));
	}

	return ret;
}

//insert the kmer into the hashtable, using linear probing to avoid collisions
void Hashtable::insert(const std::string& kmer, int loc) {
	int hash_val = hash(kmer) % table.size();

	//if the intended location is empty, add the kmer, otherwise use linear probing
	if (table[hash_val].key == "") {
		table[hash_val].key = kmer;
	} else if (table[hash_val].key != kmer) {
		while (table[hash_val].key != "" && table[hash_val].key != kmer) {
			hash_val++;
			hash_val %= table.size();
		}
		table[hash_val].key = kmer;
	}
	table[hash_val].locations.push_back(loc);
	size++;
	check_size();
}

/*Find the kmer in the table if it exists. Return a vector containing each location that the kmer is found
in the genome.*/
std::vector<int> Hashtable::find(const std::string& kmer) const {
	int hash_val = hash(kmer) % table.size();
	//search for the kmer, returning empty vector if not found
	while (table[hash_val].key != kmer) {
		if (table[hash_val].key == "") {
			return std::vector<int>();
		}
		hash_val++;
		hash_val %= table.size();
	}
	return table[hash_val].locations;
}