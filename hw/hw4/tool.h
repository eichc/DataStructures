#ifndef __tool_h_
#define __tool_h_

#include<fstream>
#include<iostream>
#include<list>
#include<string>

class Tool {
public:
	//constructors
	Tool(const std::string& m_id, const std::string& m_name, int m_num);
	Tool(const std::string& m_id, int m_num);

	//accessors
	std::string getId() const { return id; }
	std::string getName() const { return name; }
	int getAvailable() const { return num_available; }

	//modifiers
	void change_num_available(int n) { num_available += n; }

private:
	std::string id;
	std::string name;
	int num_available;
};

std::ostream& operator<< (std::ostream& out, const Tool& t);
bool operator< (const Tool& left, const Tool& right);

#endif