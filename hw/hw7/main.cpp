#include<iostream>
#include<fstream>
#include<set>
#include"fighter.h"


const std::string move_names[7] = {"jab", "forward-tilt", "up-tilt", "down-tilt", "forward-smash", "up-smash", "down-smash"};

/*Read the database file and create fighters from it, and add the fighters to a map.*/
void create_fighters(std::map<std::string, Fighter>& fighters, std::ifstream& in_str) {
	//skip the first line in the file
	std::string word;
	for (int i = 0; i < 8; ++i) {
		in_str >> word;
	}

	std::string name;
	int i = 0; //keeps track of which move to create
	while (in_str >> word) {
		//check if input is a name or a number
		if (!isdigit(word[0])) {
			name = word;
			fighters.insert(std::make_pair(name, Fighter(name)));
			i = 0;
			continue;
		}

		fighters[name].add_move(move_names[i], std::stoi(word));
		++i;
	}
}

/*Check if the inputted move is valid*/
bool check_move(const std::string& move) {
	for (int i = 0; i < 7; ++i) {
		if (move == move_names[i]) {
			return true;
		}
	}
	return false;
}

/*For the specified fighter, output the startup frame for the given move (or for all moves).*/
void query_q(std::map<std::string, Fighter>& fighters, const std::string& name, const std::string& move,
	std::ofstream& out_str) {
	if (move != "all") {
		out_str << name << ' ' << move << ": ";
		out_str << fighters[name].get_move(move) << std::endl;
		return;
	}

	//recursively call query_q for every move
	std::map<std::string, int> moves = fighters[name].Moves();
	std::map<std::string, int>::const_iterator itr;
	for (itr = moves.begin(); itr != moves.end(); ++itr) {
		query_q(fighters, name, itr->first, out_str);
	}
}

/*Output limit number of fighters that have the fastest or slowest startup frame for the specified move.
If reverse is true, the slowest are output and if reverse is false the fastest are output.*/
void query_fs(std::map<std::string, Fighter>& fighters, const std::string& move, const std::string& limit,
	bool reverse, std::ofstream& out_str) {
	//sort the fighters by the given move
	std::set<Fighter> sorted_fighters;
	std::map<std::string, Fighter>::iterator m_itr;
	for (m_itr = fighters.begin(); m_itr != fighters.end(); ++m_itr) {
		m_itr->second.change_comparison(move, reverse);
		sorted_fighters.insert(m_itr->second);
	}

	//print the appropriate number of fighters
	std::set<Fighter>::const_iterator s_itr = sorted_fighters.begin();
	for (unsigned int i = 0; i < std::min(std::stoul(limit), sorted_fighters.size()); ++i) {
		out_str << s_itr->Name() << ' ' << s_itr->get_move(move) << std::endl;
		s_itr++;
	}
}

/*Output every fighter that has the given startup frame for the given move.*/
void query_d(std::map<std::string, Fighter>& fighters, const std::string& move, const std::string& frame,
	std::ofstream& out_str) {
	//add the appropriate fighters to a set
	std::set<Fighter> new_fighters;
	std::map<std::string, Fighter>::iterator m_itr;
	for (m_itr = fighters.begin(); m_itr != fighters.end(); ++m_itr) {
		if (m_itr->second.get_move(move) == stoi(frame)) {
			new_fighters.insert(m_itr->second);
		}
	}

	//print the set
	std::set<Fighter>::const_iterator s_itr;
	for (s_itr = new_fighters.begin(); s_itr != new_fighters.end(); ++s_itr) {
		out_str << s_itr->Name() << std::endl;
	}
}

int main(int argc, char** argv){
	// Argument parsing
	if(argc != 4){
		std::cerr << "Proper usage is " << argv[0] << " [database file] [input file] [output file]" << std::endl;
		return -1;
	}

	std::ifstream dbfile(argv[1]);
	if(!dbfile){
		std::cerr << "Failed to open database " << argv[1] << " for reading." << std::endl;
	}

	std::ifstream infile(argv[2]);
	if(!infile){
		std::cerr << "Failed to open input " << argv[2] << " for reading." << std::endl;
	}

	std::ofstream outfile(argv[3]);
	if(!outfile){
		std::cerr << "Failed to open output " << argv[3] << " for writing." << std::endl;
	}

	//fill in map of fighters from database file
	std::map<std::string, Fighter> fighters;
	create_fighters(fighters, dbfile);

	//read queries
	std::string option, arg1, arg2;
	while (infile >> option >> arg1 >> arg2) {
		if (option == "-q") {
			
			std::map<std::string, Fighter>::const_iterator itr;
			itr = fighters.find(arg1);
			if (itr == fighters.end()) {
				outfile << "Invalid character name: " << arg1 << std::endl << std::endl;
				continue;
			}
			if (!check_move(arg2) && arg2 != "all") {
				outfile << "Invalid move name: " << arg2 << std::endl << std::endl;
				continue;
			}
			query_q(fighters, arg1, arg2, outfile);

		} else {
			if (!check_move(arg1)) {
				outfile << "Invalid move name: " << arg1 << std::endl << std::endl;
				continue;
			}
			outfile << option << ' ' << arg1 << ' ' << arg2 << std::endl;

			if (option == "-f") {
				query_fs(fighters, arg1, arg2, false, outfile);
			} else if (option == "-s") {
				query_fs(fighters, arg1, arg2, true, outfile);
			} else {
				query_d(fighters, arg1, arg2, outfile);
			}
		}
		outfile << std::endl;
	}


	return 0;
}
