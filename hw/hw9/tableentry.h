#ifndef tableentry_h_
#define tableentry_h_

//mini helper class for the hashtable that represents each entry in the table
class TableEntry {
public:
	friend class Hashtable;

private:
	std::string key;
	std::vector<int> locations;
};

#endif