#ifndef HeatMap_H		// Double inclusion protection
#define HeatMap_H

#include <iostream>
#include <vector>

using namespace std;

class HeatMap {
	public:
		HeatMap();
		~HeatMap() {};
		void GetShot(int& rowToShoot, int& colToShoot);
		void initializeHeatMap(int sizeOfBoard);
		void generateProbability(int playerBoard[10][10]);
		void addShip(int shipSize);
		void printHeatMap();
	private:
		int map[10][10];
		int basicProbabilityMap[10][10];
		int boardSize;
		vector<int> shipLengths;		
};

#endif
