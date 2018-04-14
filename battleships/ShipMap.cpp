/**
 * @brief Cheddar AI ShipMap for Battleships
 * @file ShipMap.cpp
 * @authors Jordan Wood, David Fletcher, Ryan Houck
 * @date April 14, 2018
 *
 * This class generates and maintains a map of where the enemy has
 * shot. It can also give the index positions of the best place to
 * put a ship of a given length, considering an opponent's prior
 * shooting pattern.
 */

#include "ShipMap.h"

using namespace std;

/**
 * @brief Initializes the ship map.
 * @param boardSz The size of the Battleships board.
 */
void ShipMap::initializeShipMap(int boardSz){
	boardSize = boardSz;
}

/**
 * @brief Subtracts one from a map coordinate if it was shot at and adds three if it wasn't shot at.
 * @param board The board that kept track of where the opponent shot last game.
 */
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

/**
 * @brief Resets the current ship placement to map.
 */
void ShipMap::resetCurShipPlacement() {
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			curShipPlacement[i][j] = map[i][j];
		}
	}
}

/**
 * @brief Gives the row, column, and direction of the best location to place a ship.
 * @param shipSize The length of the ship.
 * @param row The pointer to the row index at which a ship should be placed (ByRef).
 * @param col The pointer to the col index at which a ship should be placed (ByRef).
 * @param direction The pointer to the orientation at which a ship should be placed (0 = horizontal, 1 = vertical) (ByRef).
 */
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

/**
 * @brief Places a ship on the current ship placement map.
 * @param r The row that the ship starts on.
 * @param c The column that the ship starts on.
 * @param dir The direction that the ship is facing (0 = horizontal, 1 = vertical).
 * @param shipSz The size of the ship.
 */
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

