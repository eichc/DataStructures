// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//
// =======================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>

#include "traincar.h"

// =======================================================================
// =======================================================================
//
//  HERE'S WHERE YOU SHOULD IMPLEMENT THE FUNCTIONS YOU PROTOTYPED IN
//  THE "traincar_prototypes.h" FILE.
//


/*Using the given train, update the total weight of the train and the number of each type of car.*/
void TotalWeightAndCountCars(TrainCar* train, int& weight, int& num_e, int& num_f, int& num_p,
	int& num_d, int& num_s) {
	weight = num_e = num_f = num_p = num_d = num_s = 0;
	TrainCar* temp = train;
	while (temp != NULL) {
		weight += temp->getWeight();
		if (temp->isEngine()) {
			num_e++;
		} else if (temp->isFreightCar()) {
			num_f++;
		} else if (temp->isPassengerCar()) {
			num_p++;
		} else if (temp->isDiningCar()) {
			num_d++;
		} else {
			num_s++;
		}
		temp = temp->next;
	}
}

/*Calculate the maximum speed of the train on a 2% incline.*/
float CalculateSpeed(TrainCar* train) {
	int weight, num_e, num_f, num_p, num_d, num_s;
	TotalWeightAndCountCars(train, weight, num_e, num_f, num_p, num_d, num_s);
	float hp = 3000*num_e;
	return (75*hp)/(8*weight); //simplified from formula given in hw document
}

/*Calculate the average distance between each passenger car and a dining car. Return
a negative number if a passenger car is blocked by an engine or freight car*/
float AverageDistanceToDiningCar(TrainCar* train) {
	float total_dist = 0;
	int dist_left = 0, dist_right = 0, num_pass = 0;
	TrainCar* current = train;
	TrainCar* left;
	TrainCar* right;
	while (current != NULL) {
		//ignore all cars that aren't passengers
		if (!(current->isPassengerCar())) {
			current = current->next;
			continue;
		}
		num_pass++;

		//loop both forwads and backwards for each passenger car to find the closest dining
		dist_right = 0;
		right = current->next;
		while (right != NULL) {
			if (right->isEngine() || right->isFreightCar()) {
				dist_right = -1;
				break;
			}
			dist_right++;
			if (right->isDiningCar()) {
				break;
			}
			right = right->next;
		}

		dist_left = 0;
		left = current->prev;
		while (left != NULL) {
			if (left->isEngine() || left->isFreightCar()) {
				dist_left = -1;
				break;
			}
			dist_left++;
			if (left->isDiningCar()) {
				break;
			}
			left = left->prev;
		}

		//determine shortest distance from dining
		if (std::max(dist_left, dist_right) < 0) {
			return -1;
		}
		if (dist_left < 0) {
			total_dist += dist_right;
		} else if (dist_right < 0) {
			total_dist += dist_left;
		} else {
			total_dist += std::min(dist_left, dist_right);
		}

		current = current->next;
	}
	return total_dist/num_pass;
}

/*Calculate the shortest distance between an engine and sleeping car across the entire train.*/
int ClosestEngineToSleeperCar(TrainCar* train) {
	int closest_dist = -1, curr_dist = 0;
	TrainCar* current = train;
	bool is_eng = false;
	while (current->next != NULL) {
		//loop forwards from each engine and sleeper
		if (current->isEngine()) {
			is_eng = true;
		} else if (current->isSleepingCar()) {
			is_eng = false;
		} else {
			current = current->next;
			continue;
		}

		//find closest sleeper/engine to the current engine/sleeper
		TrainCar* search = current;
		curr_dist = 0;
		while (search != NULL) {
			if (is_eng && search->isSleepingCar()) {
				break;
			} else if (!is_eng && search->isEngine()) {
				break;
			}
			curr_dist++;
			search = search->next;
		}
		if (closest_dist < 0 || curr_dist < closest_dist) {
			closest_dist = curr_dist;
		}

		current = current->next;
	}
	return closest_dist;
}

/*Add a new car to the end of the train.*/
void PushBack(TrainCar*& train, TrainCar* car) {
	//edge case for when the train is empty
	if (train == NULL) {
		train = car;
		return;
	}

	//find the final car in the train
	TrainCar* temp = train;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	//connect the final car to the new car
	temp->next = car;
	car->prev = temp;
}

/*Delete all train cars connected to the train by continuously removing the end car.*/
void DeleteAllCars(TrainCar*& train) {
	while (train != NULL) {
		//if there is only one car left
		if (train->next == NULL) {
			delete train;
			train = NULL;
			return;
		}

		//delete the final car
		TrainCar* temp = train;
		while (temp->next->next != NULL) {
			temp = temp->next;
		}
		delete temp->next;
		temp->next = NULL;
	}
}

/*Given a train of engines and a train of freight cars, separate them into the smallest number
of trains with one or two engines each, maintaining the given minimum speed and maximum number 
of cars.*/
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights, float min_speed, 
	int max_cars) {
	int max_weight;
	std::vector<TrainCar*> trains;
	TrainCar* curr_engine = NULL;
	TrainCar* curr_freight = NULL;
	TrainCar* curr_train = NULL;
	int curr_length, curr_weight, freight_length;

	//one train is created per loop
	while (engines != NULL && freights != NULL) {
		curr_train = NULL;
		curr_freight = freights;
		curr_length = curr_weight = 0;

		//determine how many cars are left in freights for optimization
		TrainCar* temp = freights;
		freight_length = 0;
		while (temp != NULL) {
			freight_length++;
			temp = temp->next;
		}

		//remove the next engine(s) to be used from engines, and start a new train with it/them
		//use either 1 or 2 engines depending on how many freight cars are left
		int num_engines = 2;
		if (freight_length <= 4) {
			num_engines = 1;
		}
		for (int i = 0; i < num_engines; i++) {
			curr_engine = engines;
			engines = engines->next;
			if (engines != NULL) {
				engines->prev = NULL;
			}
			curr_engine->next = NULL;
			curr_engine->prev = NULL;
			PushBack(curr_train, curr_engine);
			curr_length++;
			curr_weight += 150;
		}
		

		//add freight cars to the train, removing them from freights
		max_weight = (num_engines*28125)/min_speed;
		while (freights != NULL && (curr_weight + curr_freight->getWeight()) <= max_weight && 
			curr_length < max_cars) {
			freights = freights->next;
			if (freights != NULL) {
				freights->prev = NULL;
			}
			curr_freight->next = NULL;
			curr_freight->prev = NULL;
			PushBack(curr_train, curr_freight);
			curr_length++;
			curr_weight += curr_freight->getWeight();
			curr_freight = freights;
		}
		trains.push_back(curr_train);
	}


	return trains;
}

/*Separate one train into two new trains, with speeds similar to the original. Minimize the cost
by minimizing the number of links, unlinks, and shifts.*/
void Separate(TrainCar*& original, TrainCar*& left, TrainCar*& right) {
	int weight, num_e, num_f, num_p, num_d, num_s;
	TotalWeightAndCountCars(original, weight, num_e, num_f, num_p, num_d, num_s);
	int num_cars = num_f + num_p + num_d + num_s;
	int left_eng = 0, right_eng = 0, left_cars = 0, right_cars = 0;
	int left_eng_goal, right_eng_goal, left_cars_goal, right_cars_goal;

	//determine how many cars/engines will go in left train vs right to maintain original speed
	left_eng_goal = num_e/2;
	right_eng_goal = num_e - left_eng_goal;
	float ratio = (float)left_eng_goal/num_e;
	left_cars_goal = ratio*num_cars;
	right_cars_goal = num_cars - left_cars_goal;

	//distribute the cars into their trains
	left = right = NULL;
	while (left_eng < left_eng_goal || right_eng < right_eng_goal || left_cars < left_cars_goal
		|| right_cars < right_cars_goal) {
		TrainCar* temp = original;
		original =  original->next;
		temp->next = NULL;
		temp->prev = NULL;

		/*by default, the left train will be the smaller train. However, if it will cost less 
		for the right train to be the smaller train, do that.*/
		if (left_cars == left_cars_goal && left_eng != left_eng_goal && !(temp->isEngine())) {
			int helper = left_cars_goal;
			left_cars_goal = right_cars_goal;
			right_cars_goal = helper;

			helper = left_eng_goal;
			left_eng_goal = right_eng_goal;
			right_eng_goal = helper;
		}

		//add the current car or engine to its appropriate train
		if (temp->isEngine() && left_eng < left_eng_goal) {
			PushBack(left, temp);
			left_eng++;
		} else if (temp->isEngine()) {
			PushBack(right, temp);
			right_eng++;
		} else if (left_cars < left_cars_goal) {
			PushBack(left, temp);
			left_cars++;
		} else {
			PushBack(right, temp);
			right_cars++;
		}
	}
}