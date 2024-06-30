#ifndef __player_h_
#define __player_h_

#include<string>

class Player {
public:
	//constructor with default arguments
	Player(const std::string& aName="", const std::string& aTeam="");

	//accessors
	const std::string& getName() const;
	const std::string& getTeam() const;
	int getGoals() const;
	int getAssists() const;
	int getPenalties() const;

	//modifiers
	void addGoal(int g);
	void addAssist(int a);
	void addPenalty(int p);


private:
	std::string name;
	std::string team_name;
	int goals;
	int assists;
	int penalties;
};
// operator< to allow sorting
bool operator< (const Player& left, const Player& right);

#endif