#include"customer.h"

//constrctor with name and id arguments
Customer::Customer(const std::string& m_id, const std::string& m_name) {
	id = m_id;
	name = m_name;
}

//add a tool to the rentals list
void Customer::add_rental(const Tool& t) {
	for (std::list<Tool>::iterator itr = rentals.begin(); itr != rentals.end(); ++itr) {
		if (itr->getId() == t.getId()) {
			itr->change_num_available(t.getAvailable());
			return;
		}
	}
	rentals.push_back(Tool(t.getId(), t.getName(), t.getAvailable()));
}

//add a tool to the pending list
void Customer::add_pending(const Tool& t) {
	for (std::list<Tool>::iterator itr = pending.begin(); itr != pending.end(); ++itr) {
		if (itr->getId() == t.getId()) {
			itr->change_num_available(t.getAvailable());
			return;
		}
	}
	pending.push_back(Tool(t.getId(), t.getName(), t.getAvailable()));
}

//remove a tool from the rentals list
void Customer::remove_rental(const std::string& t_id, int num) {
	for (std::list<Tool>::iterator itr = rentals.begin(); itr != rentals.end(); ++itr) {
		if (itr->getId() == t_id) {
			if (itr->getAvailable() > num) {
				itr->change_num_available(num*(-1));
				return;
			} else {
				rentals.erase(itr);
				return;
			}
		}
	}
}

//remove a tool from the pending list
void Customer::remove_pending(const std::string& t_id, int num) {
	for (std::list<Tool>::iterator itr = pending.begin(); itr != pending.end(); ++itr) {
		if (itr->getId() == t_id) {
			if (itr->getAvailable() > num) {
				itr->change_num_available(num*(-1));
				return;
			} else {
				pending.erase(itr);
				return;
			}
		}
	}
}

//use << to print the details of the customer
std::ostream& operator<< (std::ostream& out, const Customer& c) {
	out << c.getId() << ' ' << c.getName() << std::endl;
	if (c.numRentals() > 0) {
		std::list<Tool> rent = c.getRentals();
		out << "Rentals: ";
		for (std::list<Tool>::iterator itr = rent.begin(); itr != rent.end(); ++itr) {
			out << itr->getId() << " (" << itr->getAvailable() << ") ";
		}
		out << std::endl;
	}
	if (c.numPending() > 0) {
		std::list<Tool> pend = c.getPending();
		out << "Pending: ";
		for (std::list<Tool>::iterator itr = pend.begin(); itr != pend.end(); ++itr) {
			out << itr->getId() << " (" << itr->getAvailable() << ") ";
		}
		out << std::endl;
	}

	return out;
}

//allow comparison of tools using <. Sort using id number.
bool operator< (const Customer& left, const Customer& right) {
	return (left.getId() < right.getId());
}