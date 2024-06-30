#include "player.h"

//constructor with player and team name arguments
Player::Player(const std::string& aName, const std::string& aTeam) {
	name = aName;
	team_name = aTeam;
	goals = 0;
	assists = 0;
	penalties = 0;
}

const std::string& Player::getName() const {
	return name;
}

const std::string& Player::getTeam() const {
	return team_name;
}

int Player::getGoals() const {
	return goals;
}

int Player::getAssists() const {
	return assists;
}

int Player::getPenalties() const {
	return penalties;
}

//add the specified number of goals
void Player::addGoal(int g) {
	goals += g;
}

//add the specified number of assists
void Player::addAssist(int a) {
	assists += a;
}

//add the specified number of penalties
void Player::addPenalty(int p) {
	penalties += p;
}

// sort first by most total points, then by least penalties, then by name
bool operator< (const Player& left, const Player& right) {
	int l_points = left.getGoals() + left.getAssists();
	int r_points = right.getGoals() + right.getAssists();
	if (l_points != r_points) {
		return l_points > r_points;
	} else if (right.getPenalties() != left.getPenalties()) {
		return left.getPenalties() < right.getPenalties();
	} else {
		return left.getName() < right.getName();
	}
}