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
	
	generateProbability(3);
	generateProbability(4);
	generateProbability(5);

	
	for(i = 0; i < boardSize; i++){
		for(j = 0; j < boardSize; j++){
			cout << basicProbabilityMap[i][j] << " ";
		}
		cout << "" << endl;
	}
	cout << flush;
	
}

void HeatMap::generateProbability(int shipSize){
	int i;
	int j;
	int k;
	//Horizontal ships
	for(i = 0; i < boardSize - shipSize + 1; i++){
		for(j = 0; j < boardSize; j++){
			for(k = 0; k < shipSize; k++){
				basicProbabilityMap[i+k][j] += 1;
			}
		}
	}
	//Vertical ships
	for(i = 0; i < boardSize; i++){
		for(j = 0; j < boardSize - shipSize + 1; j++){
			for(k = 0; k < shipSize; k++){
				basicProbabilityMap[i][j+k] += 1;
			}
		}
	}
}

int main() {
	HeatMap heatMap;
	heatMap.initializeHeatMap(10);
	return 0;
}
