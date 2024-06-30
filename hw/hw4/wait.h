#ifndef __wait_h_
#define __wait_h_

#include<list>
#include<string>

/*Helper class that represents an item on the wait list. An "item" consists of a customer
id, tool id, and amount requested.*/
class Wait {
public:
	//constructor
	Wait(const std::string& c, const std::string& t, int num);

	//accessors
	std::string getCustomer() const { return customer; }
	std::string getTool() const { return tool; }
	int getCount() const { return count; }

private:
	std::string customer;
	std::string tool;
	int count;
};

#endif