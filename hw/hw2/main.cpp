#include<algorithm>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>
#include "team.h"

//check if the command line arguments are all valid, and return an error if not
void check_args(int num_args, char* cl_args[]) {
	//check for correct # of command line arguments
	if (num_args != 4) {
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

    //check if the third argument is valid
    if ((std::string(cl_args[3]) != std::string("--team_stats")) && 
    	(std::string(cl_args[3]) != std::string("--player_stats")) && 
    	(std::string(cl_args[3]) != std::string("--custom_stats"))) {
    		std::cerr << "ERROR:  Third argument invalid." << std::endl;
		exit(1);
    	}

    return;
}

//if the given team name doesn't exist in the teams vector, create a new team and add it
void add_team(std::vector<Team>& teams, const std::string& t_name) {
	bool flag = false;
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (t_name == teams[i].getName()) {
			flag = true;
		}
	}
	//create the team
	if (!flag) {
		teams.push_back(Team(t_name));
	}
}

//if the given player doesn't exist in the given team, add them to the team
void add_player(std::vector<Team>& teams, const std::string& p_name,
	const std::string& t_name) {
	//find the correct team
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (t_name == teams[i].getName()) {
			//check if the player exists
			bool flag = false;
			std::vector<Player> players = teams[i].getPlayers();
			for (unsigned int j = 0; j < players.size(); j++) {
				if (p_name == players[j].getName()) {
					flag = true;
				}
			}
			//add the player if they don't exist
			if (!flag) {
				teams[i].addPlayer(Player(p_name, t_name));
			}
		}
	}
}

//add a penalty, goal, or assist to the given player on the given team.
void add_event(std::vector<Team>& teams, const std::string& p_name,
	const std::string& t_name, const std::string& event) {
	//find the correct team
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (t_name == teams[i].getName()) {
			//find the correct player
			std::vector<Player> players = teams[i].getPlayers();
			for (unsigned int j = 0; j < players.size(); j++) {
				if (p_name == players[j].getName()) {
					//add the event
					if (event == "penalty") {
						teams[i].addPenaltyToPlayer(p_name, 1);
					} else if (event == "goal") {
						teams[i].addGoalToPlayer(p_name, 1);
					} else {
						teams[i].addAssistToPlayer(p_name, 1);
					}
				}
			}
		}
	}
}

//used for edge case where a penalty isn't assigned to a specific player
void add_bench_penalty(std::vector<Team>& teams, const std::string& t_name) {
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (t_name == teams[i].getName()) {
			teams[i].addBenchPenalty(1);
		}
	}
}

//given two teams and two scores, add a win/loss/tie to each team as appropriate
void add_result(std::vector<Team>& teams, const std::string& t1, 
	std::string& s1, const std::string& t2, std::string& s2) {
	int score1 = std::stoi(s1);
	int score2 = std::stoi(s2);

	//find the correct teams
	for (unsigned int i = 0; i < teams.size(); i++) {
		for (unsigned int j = 0; j < teams.size(); j++) {
			if (t1 == teams[i].getName() && t2 == teams[j].getName()) {
				//assign the result to the each team
				if (score1 > score2) {
					teams[i].addWin(1);
					teams[j].addLoss(1);
				} else if (score2 > score1) {
					teams[i].addLoss(1);
					teams[j].addWin(1);
				} else {
					teams[i].addTie(1);
					teams[j].addTie(1);
				}
			}
		}
	}
}

/* read the data from a single game and update the teams & penalties vectors with the
information that has been read. */
void read_game(std::ifstream& in_str, std::ofstream& out_str, std::vector<Team>& teams,
	std::vector<std::string>& penalties) {

	//verify that the data being read starts with a date (meaning it is the start
	//of a game)
	const std::vector<std::string> days = {"Sunday,", "Monday,", "Tuesday,", "Wednesday,", 
	"Thursday,", "Friday,", "Saturday,"};
	std::string word;
	in_str >> word;
	bool found = false;
	for (unsigned int i = 0; i < days.size(); i++) {
		if (word == days[i]) {
			found = true;
		}
	}
	if (!found) {
		return;
	}

	//read the rest of the game
	std::string previous = ""; //saves the previous word that was read
	std::string curr_team = ""; //saves the team being considered
	std::string curr_event = ""; //saves if a goal or penalty is being considered
	while ((in_str >> word) && (word != "FINAL")) {
		//create the two teams if they don't exist already
		if (word == "at" || word == "vs.") {
			add_team(teams, previous);
		} else if (previous == "at" || previous == "vs.") {
			add_team(teams, word);
		}

		//deal with the data regarding penalties
		if (word == "penalty") {
			curr_event = word;
			curr_team = previous;
		}
		if (previous == "penalty" && word != "BENCH") { 
			//add the player to the team if they don't exist
			add_player(teams, word, curr_team);
			//add the penalty to the player
			add_event(teams, word, curr_team, previous);
		}
		if (word == "BENCH") {
			add_bench_penalty(teams, curr_team);
		}
		//if the current word is the name of a penalty, add it to penalties
		if (((previous.find(':') == 1) || (previous.find(':') == 2)) && 
			(curr_event == "penalty")) {
			penalties.push_back(word);
			curr_event = ""; //reset the current event after collecting all data from this penalty
		}

		//deal with the data regarding goals
		if (word == "goal") {
			curr_event = word;
			curr_team = previous;
		}
		if (previous == "goal") {
			//add the player to the team if they don't exist
			add_player(teams, word, curr_team);
			//add the goal to the player
			add_event(teams, word, curr_team, previous);
		}

		//deal with data regarding assists
		if (word == "(") {
			while ((in_str >> word) && (word != ")")) {
				add_player(teams, word, curr_team);
				add_event(teams, word, curr_team, "assist");
			}
		}

		previous = word;
	}

	//determine the result of the game
	std::string team1, score1, team2, score2;
	in_str >> team1 >> score1 >> team2 >> score2;
	add_result(teams, team1, score1, team2, score2);
}

//convert a vector of repeated penalties into two vectors, one with names and one with counts
void convert_penalties(std::vector<std::string>& penalties, 
	std::vector<std::string>& penalty_names, std::vector<int>& penalty_counts) {
	int count = 0;
	std::string previous = "";
	for (unsigned int i = 0; i < penalties.size(); i++) {
		if (penalties[i] != previous) {
			//reset the count and add the new penalty to the names
			if (count > 0) { //prevents a 0 being added the first time the loop runs
				penalty_counts.push_back(count);
			}
			penalty_names.push_back(penalties[i]);
			count = 1;
		} else {
			count++;
		}
		previous = penalties[i];
	}
	//add the final count because the loop ends before adding it
	penalty_counts.push_back(count);
}

//print the team statistics to the output file
void print_teams(std::ofstream& out_str, const std::vector<Team>& teams) {
	//find the longest team name
	unsigned int max_len = 1;
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (teams[i].getName().size() > max_len) {
			max_len = teams[i].getName().size();
		}
	}

	//header
	out_str << "Team Name" << std::string(max_len - 5, ' ') <<
	 "W   L   T   Win%  Goals  Penalties" << std::endl;

	//print each team's stats
	out_str.precision(2);
	for (unsigned int i = 0; i < teams.size(); i++) {
		out_str << teams[i].getName() << std::string(max_len-teams[i].getName().size(), ' ');
		out_str << std::setw(5) << teams[i].getWins();
		out_str << std::setw(4) << teams[i].getLosses();
		out_str << std::setw(4) << teams[i].getTies();
		out_str << std::fixed << std::setw(7) << teams[i].getWinPercent();
		out_str << std::setw(7) << teams[i].getTotalGoals();
		out_str << std::setw(11) << teams[i].getTotalPenalties();
		out_str << std::endl;
	}
}

//print the player statistics to the output file
void print_players(std::ofstream& out_str, const std::vector<Team>& teams) {
	//add all players to a single vector and sort it
	std::vector<Player> players;
	for (unsigned int i = 0; i < teams.size(); i++) {
		for (unsigned int j = 0; j < teams[i].getPlayers().size(); j++) {
			players.push_back(teams[i].getPlayers()[j]);
		}
	}
	std::sort(players.begin(), players.end());

	//find the longest player name
	unsigned int max_p_len = 1;
	for (unsigned int i = 0; i < players.size(); i++) {
		if (max_p_len < players[i].getName().size()) {
			max_p_len = players[i].getName().size();
		}
	}

	//find the longest team name
	unsigned int max_t_len = 1;
	for (unsigned int i = 0; i < teams.size(); i++) {
		if (teams[i].getName().size() > max_t_len) {
			max_t_len = teams[i].getName().size();
		}
	}

	//header
	out_str << "Player Name" << std::string(max_p_len-9, ' ') << "Team" <<
	std::string(max_t_len-1, ' ') << "Goals  Assists  Penalties" << std::endl;

	//print each player's stats
	for (unsigned int i = 0; i < players.size(); i++) {
		out_str << players[i].getName();
		out_str << std::string(max_p_len-players[i].getName().size()+2, ' ');
		out_str << players[i].getTeam();
		out_str << std::string(max_t_len-players[i].getTeam().size(), ' ');
		out_str << std::setw(8) << players[i].getGoals();
		out_str << std::setw(9) << players[i].getAssists();
		out_str << std::setw(11) << players[i].getPenalties();
		out_str << std::endl;
	}
}

//print the custom statistics to the output file
void print_custom(std::ofstream& out_str, const std::vector<std::string>& penalty_names,
	const std::vector<int>& penalty_counts) {
	//Print error message and exit function if there are no penalties
	if (penalty_names.size() == 0) {
		out_str << "No penalties, statistic could not be calculated." << std::endl;
		return;
	}

	//find the longest penalty name, and at the same time count the total penalties
	unsigned int max_len = 1;
	double total = 0;
	for (unsigned int i = 0; i < penalty_names.size(); i++) {
		if (max_len < penalty_names[i].size()) {
			max_len = penalty_names[i].size();
		}
		total += penalty_counts[i];
	}

	//header
	out_str << "Penalty Name" << std::string(max_len-10, ' ') << 
	"Occurrences   % of Total" << std::endl;

	//print the penalty statistics
	out_str.precision(3);
	for (unsigned int i = 0; i < penalty_names.size(); i++) {
		out_str << penalty_names[i] << std::string(max_len-penalty_names[i].size(), ' ');
		out_str << std::setw(13) << penalty_counts[i];
		out_str << std::fixed << std::setw(13) << (penalty_counts[i]/total);
		out_str << std::endl;
	}
}

int main(int argc, char* argv[]) {
	//verify command line arguments
	check_args(argc, argv);

	//create variables for command line args
	std::ifstream in_str(argv[1]);
	std::ofstream out_str(argv[2]);
	std::string stat_option(argv[3]);

	//collect data from input file, updating teams and penalties vectors
	std::vector<std::string> penalties;
	std::vector<Team> teams;
	while (!in_str.eof()) {
		read_game(in_str, out_str, teams, penalties);
	}

	//organize custom statistic (penalties)
	std::sort(penalties.begin(), penalties.end());
	std::vector<std::string> penalty_names;
	std::vector<int> penalty_counts;
	convert_penalties(penalties, penalty_names, penalty_counts);

	//print the requested statistic
	if (stat_option == "--team_stats") {
		std::sort(teams.begin(), teams.end()); //sort the teams before trying to print them
		print_teams(out_str, teams);
	} else if (stat_option == "--player_stats") {
		print_players(out_str, teams);
	} else {
		print_custom(out_str, penalty_names, penalty_counts);
	}


	return 0;
}