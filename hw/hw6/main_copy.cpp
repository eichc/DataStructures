#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include"loc.h"

typedef std::vector<std::vector<char>> Grid;

/*Create vectors of the required and forbidden words from the input file.*/
void read_words(std::ifstream& in_str, std::vector<std::string>& words, 
	std::vector<std::string>& forbidden) {
	std::string symbol, word;
	while (in_str >> symbol >> word) {
		if (symbol == "+") {
			words.push_back(word);
		} else if (symbol == "-") {
			forbidden.push_back(word);
		}
	}
}

/*Print the given board of characters to the output file.*/
void print_board(const Grid& board, std::ofstream& out_str) {
	out_str << "Board: " << std::endl;
	for (unsigned int i = 0; i < board.size(); ++i) {
		out_str << "  ";
		for (unsigned int j = 0; j < board[0].size(); ++j) {
			out_str << board[i][j];
		}
		out_str << std::endl;
	}
}

/*Return true if there are no empty spaces in the board.*/
bool is_full(const Grid& board) {
	for (unsigned int i = 0; i < board.size(); ++i) {
		for (unsigned int j = 0; j < board[0].size(); ++j) {
			if (board[i][j] == '-') {
				return false;
			}
		}
	}
	return true;
}

/*Recursive version of add_word that tries to add the given word starting at loc, in the given
direction (indicated by r and c). Return true if the word was added.*/
bool add_word(Grid& board, const std::string& word, int word_i, const Loc& loc, int r, int c) {
	if (word_i == word.size()) {
		return true;
	}
	if (loc.row >= board.size() || loc.col >= board[0].size() || loc.row < 0 || loc.col < 0) {
		return false;
	}
	if (board[loc.row][loc.col] == '-') {
		board[loc.row][loc.col] = word[word_i];
	}
	if (board[loc.row][loc.col] != word[word_i]) {
		return false;
	}
	return add_word(board, word, word_i+1, Loc(loc.row+r, loc.col+c), r, c);

}

/*Find all possible ways to add the given word to the board, starting at the given location.
Return a vector containing each grid that successfully added the word.*/
std::vector<Grid> add_word(Grid& board, const std::string& word, const Loc& loc) {
	std::vector<Grid> boards;
	//if the word is a single letter
	if (word.size() == 1) {
		Grid new_board = board;
		if (new_board[loc.row][loc.col] == '-') {
			new_board[loc.row][loc.col] = word[0];
		}
		if (new_board[loc.row][loc.col] == word[0]) {
			boards.push_back(new_board);
		}
		return boards;
	}

	for (int r = -1; r < 2; ++r) {
		for (int c = -1; c < 2; ++c) {
			Grid new_board = board;
			if (add_word(new_board, word, 0, loc, r, c)) {
				boards.push_back(new_board);
			}
		}
	}
	return boards;
}

/*Recursively search for the word starting at loc, branching off in all 8 directions. 
To ensure linear word search, keep track of a vector of locations in the same branch.*/
bool find_word_from_loc(const Grid& board, const std::string& word, const Loc& loc, int letter,
	std::vector<Loc>& path) {
	/*if (word == "aa") {
		std::cout << word << ' ' << loc.row << ',' << loc.col << ' ' << letter << std::endl;
		std::cout << "Path: ";
		for (unsigned int k = 0; k < path.size(); ++k) {
			std::cout << path[k].row << ',' << path[k].col << ' ';
		}
		std::cout << std::endl;
	}*/
	if (letter == word.size()) {
		return true;
	}
	//if out of bounds
	if (loc.row >= board.size() || loc.col >= board[0].size() || loc.row < 0 || loc.col < 0) {
		return false;
	}
	if (word.size() == 1) {
		return word[letter] == board[loc.row][loc.col];
	}
	if (word[letter] != board[loc.row][loc.col]) {
		return false;
	}
	path.push_back(loc);
	//if searching for first letter
	if (path.size() == 1) {
		if (find_word_from_loc(board, word, Loc(loc.row-1, loc.col-1), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row-1, loc.col), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row-1, loc.col+1), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row, loc.col-1), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row, loc.col+1), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row+1, loc.col-1), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row+1, loc.col), letter+1, path)) {
			return true;
		}
		if (find_word_from_loc(board, word, Loc(loc.row+1, loc.col+1), letter+1, path)) {
			return true;
		}
		return false;
	}
	int row_change = loc.row - path[path.size()-1].row;
	int col_change = loc.col - path[path.size()-1].col;
	return find_word_from_loc(board, word, Loc(loc.row+row_change, loc.col+col_change), 
		letter+1, path);

}

/*Search the given board for any of the forbidden words, and return true if one is found.*/
bool find_forbidden(const Grid& board, const std::vector<std::string>& forbidden) {
	std::vector<Loc> path;
	for (unsigned int i = 0; i < forbidden.size(); ++i) {
		//check if the word can be found starting at each board location
		for (unsigned int r = 0; r < board.size(); ++r) {
			for (unsigned int c = 0; c < board[0].size(); ++c) {
				path.clear();
				if (find_word_from_loc(board, forbidden[i], Loc(r,c), 0, path)) {
					if (forbidden[i] == "aa") {
						std::cout << "Forbidden found: " << forbidden[i] << ' ' << r << ',' << c << std::endl;
					}
					/*std::cout << "Path: ";
					for (unsigned int k = 0; k < path.size(); ++k) {
						std::cout << path[k].row << ',' << path[k].col << ' ';
					}
					std::cout << std::endl;*/
					return true;
				}
			}
		}
	}
	return false;
}

/*Fill all empty spaces left on the board with each letter of the alphabet, and add each filled 
board to the vector of boards so long as no forbidden words are present.*/
void fill_spaces(Grid& board, std::vector<Grid>& boards, const std::vector<std::string>& forbidden) {
	std::vector<char> abc = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z'};
	if (find_forbidden(board, forbidden)) {
		std::cout << "True" << std::endl;
		return;
	}
	if (is_full(board)) {
		boards.push_back(board);
		return;
	}
	//plug every letter into the next empty space
	/*for (unsigned int i = 0; i < abc.size(); ++i) {
		for (unsigned int r = 0; r < board.size(); ++r) {
			bool flag = false;
			for (unsigned int c = 0; c < board[0].size(); ++c) {
				if (board[r][c] == '-') {
					Grid new_board = board;
					new_board[r][c] = abc[i];
					fill_spaces(new_board, boards, forbidden);
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
		}
	}*/

	for (unsigned int r = 0; r < board.size(); ++r) {
		for (unsigned int c = 0; c < board.size(); ++c) {
			if (board[r][c] == '-') {
				for (unsigned int i = 0; i < abc.size(); ++i) {
					Grid new_board = board;
					new_board[r][c] = abc[i];
					if (abc[i] == 'e') {
						std::cout << "Fill: " << std::endl;
						for (unsigned int g = 0; g < new_board.size(); ++g) {
							std::cout << "  ";
							for (unsigned int h = 0; h < new_board[0].size(); ++h) {
								std::cout << new_board[g][h];
							}
							std::cout << std::endl;
						}
					}
					fill_spaces(new_board, boards, forbidden);
				}
			}
		}
	}
}

/**/
void find_boards(Grid& board, std::vector<Grid>& boards, const std::vector<std::string>& words,
	unsigned int words_i, const std::vector<std::string>& forbidden) {
	//if all words have been added
	if (words_i == words.size()) {
		if (find_forbidden(board, forbidden)) {
			/*
			for (unsigned int i = 0; i < board.size(); ++i) {
				std::cout << "  ";
				for (unsigned int j = 0; j < board[0].size(); ++j) {
					std::cout << board[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			/**/
			return;
		} else if (is_full(board)) {
			boards.push_back(board);
			return;
		} else {
			//add each letter of alphabet but check for forbidden words
			fill_spaces(board, boards, forbidden);
			return;
		}
	}
	//add next word
	for (unsigned int r = 0; r < board.size(); ++r) {
		for (unsigned int c = 0; c < board[0].size(); ++c) {
			//if the word can start at this location
			if (board[r][c] == '-' || board[r][c] == words[words_i][0]) {
				Grid new_board = board;
				std::vector<Grid> new_boards = add_word(new_board, words[words_i], Loc(r, c));
				/*
				if (words[words_i] == "nun") {
					for (unsigned int i = 0; i < new_boards.size(); ++i) {
						std::cout << "Board: " << std::endl;
						for (unsigned int g = 0; g < new_boards[i].size(); ++g) {
							std::cout << "  ";
							for (unsigned int h = 0; h < new_boards[i][0].size(); ++h) {
								std::cout << new_boards[i][g][h];
							}
							std::cout << std::endl;
						}
					}
					std::cout << "Done" << std::endl;
				}
				/**/
				for (unsigned int i = 0; i < new_boards.size(); ++i) {
					find_boards(new_boards[i], boards, words, words_i+1, forbidden);
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {
	//check command line args
	if (argc != 4) {
		std::cerr << "ERROR:  Incorrect number of arguments." << std::endl;
		exit(1);
	}
	std::ifstream in_str(argv[1]);
	if (!in_str.good()) {
		std::cerr << "ERROR:  Invalid input file." << std::endl;
		exit(1);
	}
	std::ofstream out_str(argv[2]);
    if (!out_str.good()) {
    	std::cerr << "ERROR:  Invalid output file." << std::endl;
		exit(1);
    }
    std::string out_option = std::string(argv[3]);
    if ((out_option != "one_solution") && (out_option != "all_solutions")) {
    	std::cerr << "ERROR:  Third argument invalid." << std::endl;
		exit(1);
    }

	//initialize variables from input file
	std::string str_rows, str_cols;
	in_str >> str_cols >> str_rows;
	int rows = std::stoi(str_rows);
	int cols = std::stoi(str_cols);

	std::vector<std::string> words, forbidden;
	read_words(in_str, words, forbidden);

	Grid empty_board(rows, std::vector<char>(cols, '-'));
	std::vector<Grid> boards;

	//create the boards
	find_boards(empty_board, boards, words, 0, forbidden);

	//print the boards
	if (boards.size() == 0) {
		out_str << "No solutions found" << std::endl;
	}
	else if (out_option == "one_solution") {
		print_board(boards[0], out_str);
	} else {
		out_str << boards.size() << " solution(s)" << std::endl;
		for (unsigned int i = 0; i < boards.size(); ++i) {
			print_board(boards[i], out_str);
		}
	}


	//testing
	

	return 0;
}