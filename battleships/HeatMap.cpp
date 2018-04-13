#include "HeatMap.h"

using namespace std;

HeatMap::HeatMap(int sizeOfBoard){
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
			cout << basicProbabilityMap[i][j];
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
	for(i = 0; i < boardSize - shipSize; i++){
		for(j = 0; j < boardSize; j++){
			for(k = 0; k < shipSize; k++){
				basicProbabilityMap[i+k][j] += 1;
			}
		}
	}
}
