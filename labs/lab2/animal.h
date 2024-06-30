class Animal {
public:
	Animal(std::string aName);

	//accessors
	std::string getName() const;
	double getWeight() const;
	bool getLand() const;
	bool getWater() const;
	bool getPlants() const;
	bool getAnimals() const;

	//modifiers
	void setWeight(double &new_weight);
	void setLand(bool &aLand);
	void setWater(bool &aWater);
	void setPlants(bool &aPlants);
	void setAnimals(bool &aAnimals);

	//other functions
	bool isOmnivore() const; //true if plants and animals are both true
	bool isAmphibious() const; //true if land and water are both true

private:
	std::string name;
	double weight;
	bool land;
	bool water;
	bool plants;
	bool animals;



};