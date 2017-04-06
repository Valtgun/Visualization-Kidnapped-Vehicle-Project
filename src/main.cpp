/*
 * main.cpp
 *
 *
 *
 */

#include <iostream>
#include <ctime>
#include <iomanip>
#include <random>

#include "helper_functions.h"

using namespace std;



int main() {
	// Read map data
	Map map;
	if (!read_map_data("data/map_data.txt", map)) {
		cout << "Error: Could not open map file" << endl;
		return -1;
	}

	// Read position data
	vector<control_s> position_meas;
	if (!read_control_data("data/control_data.txt", position_meas)) {
		cout << "Error: Could not open position/control measurement file" << endl;
		return -1;
	}

	// Read ground truth data
	vector<ground_truth> gt;
	if (!read_gt_data("data/gt_data.txt", gt)) {
		cout << "Error: Could not open ground truth data file" << endl;
		return -1;
	}


	return 0;
}
