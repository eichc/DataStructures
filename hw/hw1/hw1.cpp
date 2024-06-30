/* 
@author: Cam Eich

This program takes in an input file, output file, line width, and justificaiton. The words
from the input file are put in an acsii art box with the given line width and justification
(left, right, or fully justified). The result is printed to the output file.

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctype.h>
#include <vector>
#include <string.h>


/* Check if a c-style string can be converted to an int by looping through each 
character in the string. */
bool is_number(char* my_string) {
	std::string new_string(my_string); //convert to c++ style string
	for (unsigned int i = 0; i < new_string.size(); i++) {
		if (!std::isdigit(my_string[i])) {
			return false;
		}
	}
	return true;
}

/* Check the given command line arguments to see if they are valid, and display any
neccessary error messages. */
void check_args(int num_args, char* cl_args[]) {
	//check for correct # of command line arguments
	if (num_args != 5) {
		std::cerr << "ERROR:  Incorrect number of arguments." << std::endl;
		exit(1);
	}

	//check for valid input/output files
	std::ifstream in_str(cl_args[1]);
	if (!in_str.good()) {
		std::cerr << "ERROR:  Invalid input file." << std::endl;
		exit(1);
	}
	std::ofstream out_str(cl_args[2]);
    if (!out_str.good()) {
    	std::cerr << "ERROR:  Invalid output file." << std::endl;
		exit(1);
    }

    //check if the third and fourth arguments are valid.
    if (!is_number(cl_args[3])) {
    	std::cerr << "ERROR:  Third argument invalid." << std::endl;
		exit(1);
    }
    if ((std::string(cl_args[4]) != std::string("flush_left")) && 
    	(std::string(cl_args[4]) != std::string("flush_right")) && 
    	(std::string(cl_args[4]) != std::string("full_justify"))) {
    		std::cerr << "ERROR:  Fourth argument invalid." << std::endl;
		exit(1);
    	}

    return;
} 

/* Add each word from the input file to a vector, splitting up words that are too
large for the given line width. */
std::vector<std::string> get_words(std::ifstream &in_str, unsigned int width) {
	std::vector<std::string> words;
	std::string word;
	while (in_str >> word) {
		words.push_back(word);

		//split up the word if it is longer than the line length
		unsigned int end_i = words.size()-1;
		while (words[end_i].size() > width) { //while the most recent word is too long
			std::string beg = words[end_i].substr(0, width-1) + "-";
			std::string end = words[end_i].substr(width-1, words[end_i].size()-width+2);
			words[end_i] = beg; //replace the current word with the shortened version
			words.push_back(end); //add the rest of the word to the vector
			end_i = words.size()-1;
		}
	}

	return words;
}

/* Given a vector of words and a line length, determine the words that fit on each line
and return it as a 2D vector. */
std::vector<std::vector<std::string>> make_lines(const std::vector<std::string> &words, 
	unsigned int width) {
	std::vector<std::vector<std::string>> lines;
	std::vector<std::string> empty_vec; //will represent a new line in the 2D vector
	lines.push_back(empty_vec);

	unsigned int curr_line = 0;
	unsigned int spaces_used = 0;
	for (unsigned int i = 0; i < words.size(); i++) {
		//if the next word does not fit on the current line, move to the next line
		if ((spaces_used > width) or (width-spaces_used < words[i].size())) {
			curr_line++;
			spaces_used = 0;
			lines.push_back(empty_vec);
		}
		//add the word to the current line
		lines[curr_line].push_back(words[i]);
		spaces_used += (words[i].size() + 1); //the 1 accounts for a space after the word
	}

	return lines;
}

/* Given a 2D vector of words separated into lines, combine each line into a single
string. Then, left or right justify each line based on the input. The lines are returned
as a vector of strings. */
std::vector<std::string> lr_justify(std::vector<std::vector<std::string>> &lines, 
	std::string justification, unsigned int width) {
	std::vector<std::string> j_lines;

	//turn each line into a single string
	std::vector<std::string> str_lines;
	std::string curr_line;
	for (unsigned int i = 0; i < lines.size(); i++) {
		curr_line = lines[i][0]; //add the first word of the line to the string
		//check if there is only one word on the line
		if (lines[i].size() > 1) {
			//add the rest of the words to the string
			for (unsigned int j = 1; j < lines[i].size(); j++) {
				curr_line += ' ';
				curr_line += lines[i][j];
			}
		}
		str_lines.push_back(curr_line);
	}

	//add spaces to the left or right of each line
	unsigned int spaces_left; //extra spaces per line
	for (unsigned int i = 0; i < str_lines.size(); i++) {
		spaces_left = width - str_lines[i].size();
		std::string spaces(spaces_left, ' '); //string of spaces to be added to the line
		if (justification == "flush_right") {
			str_lines[i] = spaces + str_lines[i];
		} else {
			str_lines[i] += spaces;
		}
	}

	return str_lines;
}

/* Given a 2D vector of words separated into lines, combine each line into a single
string and fully justify it. The lines are returned as a vector of strings. */
std::vector<std::string> f_justify(std::vector<std::vector<std::string>> &lines,
	unsigned int width) {
	std::vector<std::string> j_lines;
	unsigned int spaces_left;

	std::string line_str; //will represent each justified line
	for (unsigned int i = 0; i < lines.size()-1; i++) {
		//left justify lines with a single word
		if (lines[i].size() == 1) {
			spaces_left = width - lines[i][0].size();
			lines[i][0] += std::string(spaces_left, ' ');
			j_lines.push_back(lines[i][0]);
		} else {
			//calculate the number of spaces to be added to the line
			spaces_left = width;
			for (unsigned int j = 0; j < lines[i].size(); j++) {
				spaces_left -= lines[i][j].size();
			}
			//add spaces one at a time to the end of each word, from left to right
			//(excluding the last word in the line)
			unsigned int j = 0;
			while (spaces_left > 0) {
				lines[i][j] += " ";
				spaces_left--;
				j++;
				//loop back to the first word after reaching the final word
				if (j == lines[i].size()-1) {
					j = 0;
				}
			}
			//add each word to a string
			line_str = ""; //empty the line from the previous loop iteration
			for (unsigned int j = 0; j < lines[i].size(); j++) {
				line_str += lines[i][j];
			}
			j_lines.push_back(line_str);
		}
	}

	//left justify the final line by calling lr_justify
	//need to create a 2D vector in order to call lr_justify
	std::vector<std::string> final_line = lines[lines.size()-1];
	std::vector<std::vector<std::string>> final_line_v{final_line};
	std::vector<std::string> final_line_just = lr_justify(final_line_v, "flush_left", width);
	j_lines.push_back(final_line_just[0]);

	return j_lines;
}

/* Given a vector of strings, print the strings in an acsii art line box. */
void print_box(const std::vector<std::string> &lines, unsigned int width, 
	std::ofstream &out_str) {
	out_str << std::string(width + 4, '-') << std::endl; //top of box

	for (unsigned int i = 0; i < lines.size(); i++) {
		out_str << "| " << lines[i] << " |" << std::endl;
	}

	out_str << std::string(width + 4, '-') << std::endl; //bottom of box
}



int main(int argc, char* argv[]) {
	//make sure all command line args are valid
	check_args(argc, argv);

	//create variables for all command line args
	std::ifstream in_str(argv[1]);
	std::ofstream out_str(argv[2]);
	unsigned int width = std::stoi(argv[3]);
	std::string justification(argv[4]);

	//create and initialize a vector with all of the input words
	std::vector<std::string> words = get_words(in_str, width);
	
	//distribute the words into their appropriate lines
	std::vector<std::vector<std::string>> lines = make_lines(words, width);

	//turn each line into a single string, and justify the lines
	std::vector<std::string> justified_lines;
	if (justification != "full_justify") {
		justified_lines = lr_justify(lines, justification, width);
	} else {
		justified_lines = f_justify(lines, width);
	}

	//print the final result in an art box to the output file
	print_box(justified_lines, width, out_str);

	return(0);
}