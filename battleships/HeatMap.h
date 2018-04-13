#ifndef HeatMap_H		// Double inclusion protection
#define HeatMap_H

#include <iostream>

using namespace std;

class HeatMap {
	public:
		HeatMap();
		~HeatMap() {};
		void GetShot(int& rowToShoot, int& colToShoot);
		void initializeHeatMap(int sizeOfBoard);
	private:
		int map[10][10];
		int basicProbabilityMap[10][10];
		int boardSize;
		
		void generateProbability(int shipSize);
};

#endif
