#ifndef __team_h_
#define __team_h_

#include<string>
#include<vector>
#include"player.h"

class Team {
public:
	//constructor with default arguments
	Team(const std::string& aName="");

	//accessors
	const std::string& getName() const;
	const std::vector<Player>& getPlayers() const;
	int getWins() const;
	int getLosses() const;
	int getTies() const;
	double getWinPercent() const;
	int getTotalGoals() const;
	int getTotalPenalties() const;

	//modifiers
	void addPlayer(const Player& p);
	void addGoalToPlayer(const std::string& n, int g);
	void addAssistToPlayer(const std::string& n, int a);
	void addPenaltyToPlayer(const std::string& n, int p);
	void addWin(int w);
	void addLoss(int l);
	void addTie(int t);
	void addBenchPenalty(int b);


private:
	std::string name;
	std::vector<Player> players;
	int wins, losses, ties;
	int bench_penalties;

};
// operator< to allow sorting
bool operator< (const Team& left, const Team& right);

#endif