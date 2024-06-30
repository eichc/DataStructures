#ifndef __customer_h_
#define __customer_h_

#include"tool.h"

class Customer {
public:
	//constructors
	Customer(const std::string& m_id, const std::string& m_name);

	//accessors
	std::string getId() const { return id; }
	std::string getName() const { return name; }
	std::list<Tool> getRentals() const { return rentals; }
	std::list<Tool> getPending() const { return pending; }
	int numRentals() const { return rentals.size(); }
	int numPending() const { return pending.size(); }

	//modifiers
	void add_rental(const Tool& t);
	void add_pending(const Tool& t);
	void remove_rental(const std::string& t_id, int num);
	void remove_pending(const std::string& t_id, int num);

private:
	std::string id;
	std::string name;
	std::list<Tool> rentals;
	std::list<Tool> pending;
};

std::ostream& operator<< (std::ostream& out, const Customer& c);
bool operator< (const Customer& left, const Customer& right);

#endif