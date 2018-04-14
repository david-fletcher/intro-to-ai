#include "HeatMap.h"
#include <iomanip>

using namespace std;

HeatMap::HeatMap(){
	/* do setup here */
}

void HeatMap::initializeHeatMap(int sizeOfBoard) {
	boardSize = sizeOfBoard;
}

void HeatMap::generateProbability(char playerBoard[10][10]){
	resetHeatMap();
	
	int shipSize;	
	//Vertical ships
	for(unsigned int ship = 0; ship < shipLengths.size(); ship++) {
		shipSize = shipLengths.at(ship);
		for(int i = 0; i < boardSize - shipSize + 1; i++){
			for(int j = 0; j < boardSize; j++){
				bool canFit = true;
				for(int k = 0; k < shipSize; k++){
					if(playerBoard[i+k][j] == MISS || playerBoard[i+k][j] == KILL){
						basicProbabilityMap[i+k][j] = -1;
						canFit = false;
					}
				}
				if(canFit){
					for(int k = 0; k < shipSize; k++){
						basicProbabilityMap[i+k][j] += 1;
					}
				}
			}
		}
	}
	//Horizontal ships
	for(unsigned int ship = 0; ship < shipLengths.size(); ship++) {	
		shipSize = shipLengths.at(ship);
		for(int i = 0; i < boardSize; i++){
			for(int j = 0; j < boardSize - shipSize + 1; j++){
				bool canFit = true;
				for(int k = 0; k < shipSize; k++){
					if(playerBoard[i][j+k] == MISS || playerBoard[i][j+k] == KILL){
						basicProbabilityMap[i][j+k] = -1;
						canFit = false;
					}
				}
				if(canFit){
					for(int k = 0; k < shipSize; k++){
						basicProbabilityMap[i][j+k] += 1;
					}
				}
			}
		}
	}
	
}

void HeatMap::addShip(int shipSize) {
	shipLengths.push_back(shipSize);
}

void HeatMap::deleteShip(int shipSize) {
	for(unsigned int i = 0; i < shipLengths.size(); i++) {
		if(shipSize == shipLengths.at(i)) {
			shipLengths.erase(shipLengths.begin() + i);
			return;
		}
	}
}

void HeatMap::printHeatMap() {
	for(int row = 0; row < boardSize; row++) {
		for(int col = 0; col < boardSize; col++) {
			cout << setw(3) << basicProbabilityMap[row][col] << " ";
		}

		cout << endl << flush;
	}
}

void HeatMap::printShipVector() {
	for(unsigned int i = 0; i < shipLengths.size(); i++) {
		cout << "Ship: " << shipLengths.at(i) << endl << flush;
	}
}

void HeatMap::resetHeatMap() {
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			basicProbabilityMap[i][j] = 0;
		}
	}
}

void HeatMap::getShot(int& rowToShoot, int& colToShoot) {
	int maxIndex = 0;
	for(int i = 0; i < boardSize; i++) {
		for(int j = 0; j < boardSize; j++) {
			if(maxIndex < basicProbabilityMap[i][j]) {
				maxIndex = basicProbabilityMap[i][j];
				rowToShoot = i;
				colToShoot = j;
			}
		}
	}
}
/*
int main() {
	char testBoard[10][10] = 
	{{'~','~','*','~','~','~','~','~','*','~'},
	 {'~','~','~','~','~','~','~','~','~','~'},
	 {'~','~','~','~','~','~','~','~','~','~'},
	 {'~','*','~','K','~','~','*','~','~','~'},
	 {'~','~','~','K','*','~','~','~','~','~'},
	 {'~','~','~','K','~','*','~','~','~','~'},
	 {'~','~','~','K','~','~','~','~','~','~'},
	 {'~','~','~','~','*','~','~','~','~','~'},
	 {'~','~','~','~','~','~','~','~','*','~'},
	 {'~','*','~','~','~','~','~','~','~','~'}};

	HeatMap heatMap;
	heatMap.initializeHeatMap(10);
	
	heatMap.addShip(3);
	heatMap.addShip(4);
	heatMap.addShip(5);

	heatMap.deleteShip(4);

	heatMap.generateProbability(testBoard);

	heatMap.printHeatMap();

	int row = 0;
	int col = 0;

	heatMap.getShot(row, col);
	cout << "Shooting at x: " << row << " y: " << col << endl << flush;

	return 0;
}
*/
