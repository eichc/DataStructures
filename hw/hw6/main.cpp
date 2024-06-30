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
bool add_word(Grid& board, const std::string& word, unsigned int word_i, const Loc& loc, int r, int c) {
	if (word_i == word.size()) {
		return true;
	}
	unsigned int lr = loc.row; //to avoid compiler warning
	unsigned int lc = loc.col;
	if (lr >= board.size() || lc >= board[0].size() || loc.row < 0 || loc.col < 0) {
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

/*Recursive version of find_word that's called once the first letter is found. r and c represent
the amount that the row and column will change by each call, indicating the direction of the word.*/
bool find_word(const Grid& board, const std::string& word, const Loc& loc, unsigned int word_i, int r, int c) {
	if (word_i == word.size()) {
		return true;
	}
	unsigned int lr = loc.row; //to avoid compiler warning
	unsigned int lc = loc.col;
	if (lr >= board.size() || lc >= board[0].size() || loc.row < 0 || loc.col < 0) {
		return false;
	}
	if (word[word_i] != board[loc.row][loc.col]) {
		return false;
	}
	return find_word(board, word, Loc(loc.row+r, loc.col+c), word_i+1, r, c);
}

/*Return true if the given word appears in the board.*/
bool find_word(const Grid& board, const std::string& word, const Loc& loc) {
	if (word[0] == board[loc.row][loc.col]) {
		//if word is only one letter
		if (word.size() == 1) {
			return true;
		}

		//search for the rest of the word
		for (int r = -1; r < 2; ++r) {
			for (int c = -1; c < 2; ++c) {
					if (find_word(board, word, Loc(loc.row, loc.col), 0, r, c)) {
						return true;
					}
			}
		}
	}

	//the word was not found, so move to the next location
	unsigned int lr = loc.row; //to avoid compiler warning
	unsigned int lc = loc.col;
	if (lr == board.size()-1 && lc == board[0].size()-1) {
		return false;
	} else if (lc == board[0].size()) {
		//move to the next row and search for the word
		return find_word(board, word, Loc(loc.row+1, 0));
	} else {
		//search for the word at the next location
		return find_word(board, word, Loc(loc.row, loc.col+1));
	}
}

/*Search the given board for any of the forbidden words, and return true if one is found.*/
bool find_forbidden(const Grid& board, const std::vector<std::string>& forbidden) {
	for (unsigned int i = 0; i < forbidden.size(); ++i) {
		if (find_word(board, forbidden[i], Loc(0,0))) {
			return true;
		}
	}
	return false;
}

/*Fill all empty spaces left on the board with each letter of the alphabet, and add each filled 
board to the vector of boards so long as no forbidden words are present.*/
void fill_spaces(Grid& board, std::vector<Grid>& boards, const std::vector<std::string>& forbidden) {
	std::vector<char> abc = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z'};
	if (is_full(board)) {
		if (!(find_forbidden(board, forbidden))) {
			boards.push_back(board);
		}
		return;
	}
	//plug every letter into the next empty space
	for (unsigned int i = 0; i < abc.size(); ++i) {
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
	}
}

/*Find all possible boards that contain all required words and no forbidden words. Recursively
add one word at a time, testing all possible ways to add each word to the grid.*/
void find_boards(Grid& board, std::vector<Grid>& boards, const std::vector<std::string>& words,
	unsigned int words_i, const std::vector<std::string>& forbidden) {
	//if all words have been added
	if (words_i == words.size()) {
		if (find_forbidden(board, forbidden)) {
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


	return 0;
}