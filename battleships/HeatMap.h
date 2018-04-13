#include <iostream>

using namespace std;

class HeatMap {
	public:
		
		HeatMap(int sizeOfBoard);
		~HeatMap();
		void GetShot(int& rowToShoot, int& colToShoot);
	private:
		int map[10][10];
		int basicProbabilityMap[10][10];
		int boardSize;
		
		void generateProbability(int shipSize);
};
