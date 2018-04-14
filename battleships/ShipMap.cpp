#include "ShipMap.h"

using namespace std;

ShipMap::ShipMap(){
	
}

void ShipMap::initializeShipMap(int boardSz){
	boardSize = boardSz;
}

void ShipMap::convertToShipMap(char board[10][10]){
	int tempShipMap[10][10];
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			if(board[i][j] != WATER){
				tempShipMap[i][j] = 3;
			}
			else{
				tempShipMap[i][j] = -1;
			}
		}
	}

	
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			map[i][j] += tempShipMap[i][j];
		}
	}
}

void ShipMap::resetCurShipPlacement() {
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			curShipPlacement[i][j] = map[i][j];
		}
	}
}

void ShipMap::bestShipLocation(int shipSize, int& row, int& col, int& direction){
	
	int minShipVert = 99999;
	int minVertR = 0;
	int minVertC = 0;
	int minShipHoriz = 99999;
	int minHorizR = 0;
	int minHorizC = 0;

	//Vertical
	for(int i = 0; i < boardSize - shipSize + 1; i++){
		for(int j = 0; j < boardSize; j++){
			int shipSum = 0;
			for(int k = 0; k < shipSize; k++){
				shipSum += curShipPlacement[i+k][j];
			}
			if(minShipVert > shipSum){
				minShipVert = shipSum;
				minVertR = i;
				minVertC = j;
			}
		}
	}

	//Horizontal
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize - shipSize + 1; j++){
			int shipSum = 0;
			for(int k = 0; k < shipSize; k++){
				shipSum += curShipPlacement[i][j+k];
			}
			if(minShipHoriz > shipSum){
				minShipHoriz = shipSum;
				minHorizR = i;
				minHorizC = j;
			}
		}
	}
	
	if(minShipVert <= minShipHoriz){
		row = minVertR;
		col = minVertC;
		direction = 1; //1 is vertical, 0 = horizontal
	}
	else{
		row = minHorizR;
		col = minHorizC;
		direction = 0;
	}
	placeShipOnMap(row, col, direction, shipSize);


}

void ShipMap::placeShipOnMap(int r, int c, int dir, int shipSz){
	if(dir == 1){//Vertical
		for(int i = 0; i < shipSz; i++){
			curShipPlacement[r+i][c] += 99999;
		}
	}
	else{
		for(int i = 0; i < shipSz; i++){
			curShipPlacement[r][c+i] += 99999;
		}
	}
}




