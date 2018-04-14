#ifndef HeatMap_H		// Double inclusion protection
#define HeatMap_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "defines.h"

using namespace std;

class HeatMap {
	public:
		HeatMap() {};
		~HeatMap() {};
		void getShot(int& rowToShoot, int& colToShoot);
		void initializeHeatMap(int sizeOfBoard);
		void generateProbability(char playerBoard[10][10]);
		void addShip(int shipSize);
		void deleteShip(int shipSize);
		void printHeatMap();
		void printShipVector();
	private:
		void resetHeatMap();
			int basicProbabilityMap[10][10];
			int boardSize;
			vector<int> shipLengths;
};

#endif
