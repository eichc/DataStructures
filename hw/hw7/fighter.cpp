#include"fighter.h"

/*Return the startup frame for the given move, or -1 if the move doesn't exist.*/
int Fighter::get_move(const std::string& move) const {
	std::map<std::string, int>::const_iterator itr;
	itr = moves.find(move);
	if (itr == moves.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

/*Operator < allows use of sets. Fighters are sorted by whichever move is specified by Method(), then by name.*/
bool operator< (const Fighter& left, const Fighter& right) {
	std::pair<std::string, bool> lm = left.Method();
	std::pair<std::string, bool> rm = right.Method();
	if (left.get_move(lm.first) != right.get_move(rm.first)) {
		if (lm.second) { //if reverse order should be used
			return left.get_move(lm.first) > right.get_move(rm.first);
		} else {
			return left.get_move(lm.first) < right.get_move(rm.first);
		}
	}
	return left.Name() < right.Name();
}