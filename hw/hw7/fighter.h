#ifndef __fighter_h_
#define __fighter_h_

#include<string>
#include<map>

class Fighter {
public:
	//constructors
	Fighter() { name = ""; }
	Fighter(const std::string& n) { name = n; }

	//accessors
	const std::string& Name() const { return name; }
	const std::map<std::string, int>& Moves() const { return moves; }
	int get_move(const std::string& move) const;
	const std::pair<std::string, bool>& Method() const { return comparison_method; }

	//modifiers
	void add_move(const std::string& move, int frame) { moves.insert(std::make_pair(move, frame)); }
	void change_comparison(const std::string& move, bool reverse) { comparison_method = std::make_pair(move, reverse); }

private:
	std::string name;
	std::map<std::string, int> moves;
	std::pair<std::string, bool> comparison_method;
};

bool operator< (const Fighter& left, const Fighter& right);

#endif