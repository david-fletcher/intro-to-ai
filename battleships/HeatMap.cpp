#include "HeatMap.h"

using namespace std;

HeatMap::HeatMap(){
	/* do setup here */
}

void HeatMap::initializeHeatMap(int sizeOfBoard) {
	int i = 0;
	int j = 0;
	boardSize = sizeOfBoard;

	for(i = 0; i < boardSize; i++){
		for(j = 0; j < boardSize; j++){
			basicProbabilityMap[i][j] = 0;
		}
	}
}

void HeatMap::generateProbability(int playerBoard[10][10]){
	int shipSize;	
	//Vertical ships
	for(int ship = 0; ship < shipLengths.size(); ship++) {
		shipSize = shipLengths.at(ship);
		for(int i = 0; i < boardSize - shipSize + 1; i++){
			for(int j = 0; j < boardSize; j++){
				for(int k = 0; k < shipSize; k++){
					basicProbabilityMap[i+k][j] += 1;
				}
			}
		}
	}
	//Horizontal ships
	for(int ship = 0; ship < shipLengths.size(); ship++) {	
		shipSize = shipLengths.at(ship);
		for(int i = 0; i < boardSize; i++){
			for(int j = 0; j < boardSize - shipSize + 1; j++){
				for(int k = 0; k < shipSize; k++){
					basicProbabilityMap[i][j+k] += 1;
				}
			}
		}
	}
	
}

void HeatMap::addShip(int shipSize) {
	shipLengths.push_back(shipSize);
}

void HeatMap::printHeatMap() {
	for(int row = 0; row < boardSize; row++) {
		for(int col = 0; col < boardSize; col++) {
			cout << basicProbabilityMap[row][col] << " ";
		}

		cout << endl << flush;
	}
}

int main() {
	int testBoard[10][10];

	HeatMap heatMap;
	heatMap.initializeHeatMap(10);
	
	heatMap.addShip(3);
	heatMap.addShip(4);
	heatMap.addShip(5);

	heatMap.generateProbability(testBoard);

	heatMap.printHeatMap();

	return 0;
}
