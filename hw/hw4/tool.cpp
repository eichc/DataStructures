#include"tool.h"

//constrctor with name, id, and number available arguments
Tool::Tool(const std::string& m_id, const std::string& m_name, int m_num) {
	id = m_id;
	name = m_name;
	num_available = m_num;
}

//constrctor without name argument
Tool::Tool(const std::string& m_id, int m_num) {
	id = m_id;
	name = "";
	num_available = m_num;
}

//use << to print details of the tool
std::ostream& operator<< (std::ostream& out, const Tool& t) {
	out << t.getId() << ' ' << t.getAvailable() << " available " << t.getName();
	return out;
}

//allow comparison of tools using <. Sort using id number.
bool operator< (const Tool& left, const Tool& right) {
	return (left.getId() < right.getId());
}