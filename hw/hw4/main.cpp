#include<fstream>
#include<iostream>
#include"customer.h"
#include"tool.h"
#include"wait.h"

//add each tool in the inventory file to the inventory list
void read_inventory(std::ifstream& in_str, std::list<Tool>& inventory) {
	std::string id, num_str, name;
	int num;
	while (in_str >> id >> num_str >> name) {
		num = std::stoi(num_str);

		//error check for invalid id
		if (id[0] != 'T' || num < 1) {
			std::cerr << "Invalid inventory ID " << id << 
			" found in the inventory file." << std::endl;
			continue;
		}

		//put the tool in the right place in the list
		bool flag = false;
		for (std::list<Tool>::iterator itr = inventory.begin(); itr != inventory.end(); ++itr) {
			if (id < itr->getId()) {
				inventory.insert(itr, Tool(id,name,num));
				flag = true;
				break;
			}
		}
		//add tool to the end if it didn't get inserted
		if (!flag) {
			inventory.push_back(Tool(id,name,num));
		}
	}
}

/*check if a customer is already in the customers list, and if not add them to the list*/
void add_customer(std::list<Customer>& customers, const std::string& id, 
	const std::string& name) {
	//check if customer is in the list
	std::list<Customer>::iterator itr;
	for (itr = customers.begin(); itr != customers.end(); ++itr) {
		if (id == itr->getId()) {
			return;
		}
	}

	//put customer in correct spot
	bool flag = false;
	for (itr = customers.begin(); itr != customers.end(); ++itr) {
		if (id < itr->getId()) {
			customers.insert(itr, Customer(id,name));
			flag = true;
			break;
		}
	}
	//add tool to the end if it didn't get inserted
	if (!flag) {
		customers.push_back(Customer(id,name));
	}
}

/*add the tool to the customer's rentals list and remove the tools from the inventory*/
void rent_tool(std::list<Customer>& customers, std::list<Tool>& inventory, 
	const std::string& c_id, const std::string& t_id, int num) {
	//remove the tools from inventory
	std::list<Tool>::iterator i_itr;
	for (i_itr = inventory.begin(); i_itr != inventory.end(); ++i_itr) {
		if (t_id == i_itr->getId()) {
			i_itr->change_num_available(num*(-1));
		}
	}

	//add tool to customer's rentals
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		if (c_id == c_itr->getId()) {
			c_itr->add_rental(Tool(t_id,num));
		}
	}
}

/*add the tool to the customer's pending list and add a new Wait object to the waitlist*/
void add_to_waitlist(std::list<Customer>& customers, std::list<Wait>& waitlist,
	const std::string& c_id, const std::string& t_id, int num) {
	//create Wait item and add it to the waitlist
	waitlist.push_back(Wait(c_id, t_id, num));

	//add tool to customer's pending list
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		if (c_id == c_itr->getId()) {
			c_itr->add_pending(Tool(t_id,num));
		}
	}
}

/*If the customer has not rented the tool, print error message. Otherwise, adjust the
inventory list and customer's rentals list accordingly*/
void return_tool(std::list<Customer>& customers, std::list<Tool>& inventory,
	const std::string& c_id, const std::string& t_id, int num) {
	//check if customer has rented the tool, and if so remove it from rentals list
	bool flag = false;
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		if (c_id == c_itr->getId()) {
			std::list<Tool>::iterator t_itr;
			std::list<Tool> rentals = c_itr->getRentals();
			for (t_itr = rentals.begin(); t_itr != rentals.end(); ++t_itr) {
				if (t_id == t_itr->getId() && num <= t_itr->getAvailable()) {
					flag = true;
					c_itr->remove_rental(t_id,num);
				}
			}
		}
	}

	//error if tool wasn't rented
	if (!flag) {
		std::cerr << "Customer " << c_id << " attempted to return item " << t_id <<
		" which she/he did not rent." << std::endl;
		return;
	}

	//add tool back to inventory
	std::list<Tool>::iterator itr;
	for (itr = inventory.begin(); itr != inventory.end(); ++itr) {
		if (t_id == itr->getId()) {
			itr->change_num_available(num);
		}
	}
}

//remove the specified number of the specified tool from the customer's pending list
void remove_from_pending(std::list<Customer>& customers, const std::string& c_id,
	const std::string& t_id, int num) {
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		if (c_id == c_itr->getId()) {
			c_itr->remove_pending(t_id, num);
		}
	}
}

/*read the customer input file, adding and removing customers from the list as appropriate.
Additionally, update the inventory and wait lists as tools are rented and returned. */
void read_customers(std::ifstream& in_str, std::list<Customer>& customers, 
	std::list<Tool>& inventory, std::list<Wait>& waitlist) {
	std::string c_id, action, time, num_str, t_id, name;
	int num;
	while (in_str >> c_id >> action >> time >> num_str >> t_id >> name) {
		num = std::stoi(num_str);

		//error check for invalid id
		if (c_id[0] != 'C') {
			std::cerr << "Invalid customer information found for ID " << c_id << 
			" in the customer file." << std::endl;
			continue;
		}

		if (action == "rent") {
			/*check for invalid tool request, and at the same time determine if the
			customer will need to be put on the waiting list*/
			bool found = false;
			bool need_to_wait;
			std::list<Tool>::iterator itr;
			for (itr = inventory.begin(); itr != inventory.end(); ++itr) {
				if (t_id == itr->getId()) {
					found = true;
					need_to_wait = (itr->getAvailable() < num);
				}
			}
			if (!found) {
				std::cerr << "Customer " << c_id << " requested item " << t_id <<
				" which is not in the inventory." << std::endl;
				continue;
			}

			//add customer to the customers list
			add_customer(customers, c_id, name);

			//rent the tool to the customer or add them to the waiting list
			if (!need_to_wait) {
				rent_tool(customers, inventory, c_id, t_id, num);
			} else {
				add_to_waitlist(customers, waitlist, c_id, t_id, num);
			}
		} else {
			//check for invalid tool id
			bool found = false;
			std::list<Tool>::iterator t_itr;
			for (t_itr = inventory.begin(); t_itr != inventory.end(); ++t_itr) {
				if (t_id == t_itr->getId()) {
					found = true;
				}
			}
			if (!found) {
				std::cerr << "Customer " << c_id << " attempted to return item " << t_id <<
				" which is not in the inventory." << std::endl;
				continue;
			}

			//return the tool or print error message if request is invalid
			return_tool(customers, inventory, c_id, t_id, num);

			//remove customer if they are no longer active
			std::list<Customer>::iterator c_itr;
			for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
				if (c_id == c_itr->getId() && c_itr->numRentals() == 0 &&
					c_itr->numPending() == 0) {
					customers.erase(c_itr);
					break;
				}
			}

			//check if customer is waiting for returned tool
			for (t_itr = inventory.begin(); t_itr != inventory.end(); ++t_itr) {
				if (t_itr->getId() == t_id) {
					break;
					/*t_itr now points to the returned tool so it can be used in the below 
					waitlist loop*/
				}
			}
			std::list<Wait>::iterator w_itr;
			for (w_itr = waitlist.begin(); w_itr != waitlist.end(); ++w_itr) {
				if (w_itr->getTool() == t_id && w_itr->getCount() <= t_itr->getAvailable()) {
					rent_tool(customers, inventory, w_itr->getCustomer(), 
						w_itr->getTool(), w_itr->getCount());
					remove_from_pending(customers, w_itr->getCustomer(), 
						w_itr->getTool(), w_itr->getCount());
				}
			}
		}
	}
}

/*print the customers that have rented the given tool, to be used for the inventory
output file*/
void print_rental_customers(std::ofstream& out_str, std::list<Customer>& customers, 
	std::list<Tool>& inventory, const std::string& t_id) {
	int i = 0; /*will keep track of how many customers are renting the tool to avoid
	printing anything if no one is renting the tool*/
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		std::list<Tool> rentals = c_itr->getRentals();
		//determine if the customer has rented the given tool
		std::list<Tool>::iterator t_itr;
		for (t_itr = rentals.begin(); t_itr != rentals.end(); ++t_itr) {
			if (t_id == t_itr->getId()) {
				++i;
				if (i==1) {
					out_str << "Rental Customers: ";
				}
				out_str << c_itr->getId() << ' ' << c_itr->getName() << " (" <<
				t_itr->getAvailable() << ") ";
			}
		}
	}
	//check to avoid printing new line if one isn't needed
	if (i!=0) {
		out_str << std::endl;
	}
}

//same functionality as print_rental_customers, but for pending instead of rentals
void print_pending_customers(std::ofstream& out_str, std::list<Customer>& customers, 
	std::list<Tool>& inventory, const std::string& t_id) {
	int i = 0;
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		std::list<Tool> pending = c_itr->getPending();
		std::list<Tool>::iterator t_itr;
		for (t_itr = pending.begin(); t_itr != pending.end(); ++t_itr) {
			if (t_id == t_itr->getId()) {
				++i;
				if (i==1) {
					out_str << "Pending Customers: ";
				}
				out_str << c_itr->getId() << ' ' << c_itr->getName() << " (" <<
				t_itr->getAvailable() << ") ";
			}
		}
	}
	if (i!=0) {
		out_str << std::endl;
	}
}

int main(int argc, char* argv[]) {
	//error checking for command line args
	if (argc != 5) {
		std::cerr << "ERROR:  Incorrect number of arguments." << std::endl;
		exit(1);
	}
	std::ifstream t_in(argv[1]);
	std::ifstream c_in(argv[2]);
	std::ofstream t_out(argv[3]);
	std::ofstream c_out(argv[4]);
	if (!t_in.good() || !c_in.good() || !t_out.good() || !c_out.good()) {
		std::cerr << "ERROR:  Invalid file name." << std::endl;
		exit(1);
	}

	//read inventory file
	std::list<Tool> inventory;
	read_inventory(t_in, inventory);

	//read customer file
	std::list<Customer> customers;
	std::list<Wait> waitlist;
	read_customers(c_in, customers, inventory, waitlist);

	//print inventory and customers to output files
	std::list<Tool>::iterator i_itr;
	for (i_itr = inventory.begin(); i_itr != inventory.end(); ++i_itr) {
		t_out << (*i_itr) << std::endl;
		print_rental_customers(t_out, customers, inventory, i_itr->getId());
		print_pending_customers(t_out, customers, inventory, i_itr->getId());
		t_out << std::endl;
	}
	std::list<Customer>::iterator c_itr;
	for (c_itr = customers.begin(); c_itr != customers.end(); ++c_itr) {
		c_out << (*c_itr) << std::endl;
	}


	return 0;
}