// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//                 This file is #include'd from traincar.h and main.cpp
//
// =======================================================================
//
// There are a number of additional functions used in main.cpp that
// you need to declare and implement.  Study main.cpp and the provided
// output to determine the arguments, return type, and overall
// behavior.
//
// Add all required additional function prototypes here
// (you may also add your own helper function prototypes here too)
//
// Implement these functions in "traincar.cpp"
//

#include<vector>


//Statistics functions
void TotalWeightAndCountCars(TrainCar* train, int& weight, int& num_e, int& num_f, int& num_p,
	int& num_d, int& num_s);

float CalculateSpeed(TrainCar* train);

float AverageDistanceToDiningCar(TrainCar* train);

int ClosestEngineToSleeperCar(TrainCar* train);

void PushBack(TrainCar*& train, TrainCar* car);

void DeleteAllCars(TrainCar*& train);


//Algorithms to manage trains
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights, float min_speed, 
	int max_cars);

void Separate(TrainCar*& original, TrainCar*& left, TrainCar*& right);