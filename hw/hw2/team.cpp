#include<algorithm>
#include"team.h"

/* constructor with team name. Players vector not initialized because it is intended
to start empty. */
Team::Team(const std::string& aName) {
	name = aName;
	wins = 0;
	losses = 0;
	ties = 0;
	bench_penalties = 0;
}

const std::string& Team::getName() const {
	return name;
}

const std::vector<Player>& Team::getPlayers() const {
	return players;
}

int Team::getWins() const {
	return wins;
}

int Team::getLosses() const {
	return losses;
}

int Team::getTies() const {
	return ties;
}

double Team::getWinPercent() const {
	if (wins + losses + ties == 0) { //to avoid division by 0
		return 0;
	} else {
		return (wins + 0.5*ties) / (wins + losses + ties);
	}
}

int Team::getTotalGoals() const {
	int total = 0;
	for (unsigned int i = 0; i < players.size(); i++) {
		total += players[i].getGoals();
	}
	return total;
}

int Team::getTotalPenalties() const {
	int total = 0;
	for (unsigned int i = 0; i < players.size(); i++) {
		total += players[i].getPenalties();
	}
	total += bench_penalties;
	return total;
}

void Team::addPlayer(const Player& p) {
	players.push_back(p);
}

//add the specified number of goals to the player with the specified name
void Team::addGoalToPlayer(const std::string& n, int g) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].getName() == n) {
			players[i].addGoal(g);
			break;
		}
	}
}

//add the specified number of assists to the player with the specified name
void Team::addAssistToPlayer(const std::string& n, int a) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].getName() == n) {
			players[i].addAssist(a);
			break;
		}
	}
}

//add the specified number of penalties to the player with the specified name
void Team::addPenaltyToPlayer(const std::string& n, int p) {
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i].getName() == n) {
			players[i].addPenalty(p);
			break;
		}
	}
}

//add the specified number of wins
void Team::addWin(int w) {
	wins += w;
}

//add the specified number of losses
void Team::addLoss(int l) {
	losses += l;
}

//add the specified number of ties
void Team::addTie(int t) {
	ties += t;
}

/* add the specified number of bench penalties (special case where the penalty
isn't assigned to a specific player) */
void Team::addBenchPenalty(int b) {
	bench_penalties += b;
}

//sort first by highest win percentage, then by most total goals, then by team name
bool operator< (const Team& left, const Team& right) {
	if (left.getWinPercent() != right.getWinPercent()) {
		return left.getWinPercent() > right.getWinPercent();
	} else if (left.getTotalGoals() != right.getTotalGoals()) {
		return left.getTotalGoals() > right.getTotalGoals();
	} else {
		return left.getName() < right.getName();
	}
	
}