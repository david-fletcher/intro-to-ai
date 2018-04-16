/**
 * @brief Cheddar AI HeatMap for Battleships
 * @file HeatMap.cpp
 * @authors Jordan Wood, David Fletcher, Ryan Houck
 * @date April 14, 2018
 *
 * This class generates and maintains a heatmap of the probability
 * of a ship being in any given location. It can also give the index
 * positions of the most likely location for a ship to be.
 */
#include "HeatMap.h"

using namespace std;

/**
 * @brief Initializes the heatmap.
 * @param sizeOfBoard The specified size of the Battleships board.
 */
void HeatMap::initializeHeatMap(int sizeOfBoard) {
	boardSize = sizeOfBoard;
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			longRunningMap[i][j] = 0;
		}
	}
}

/**
 * @brief Generates the probability heatmap of a ship being at any location for the given boardstate.
 * @param playerBoard The board being shot at (containing the opponent's ships).
 */
void HeatMap::generateProbability(char playerBoard[10][10]){
	resetHeatMap();
	addMapsTogether();

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
						basicProbabilityMap[i][j+k] += 2;
					}
				}
			}
		}
	}
	
}

/**
 * @brief Adds a ship to the shipLengths vector.
 * @param shipSize The length of the ship being added.
 */
void HeatMap::addShip(int shipSize) {
	shipLengths.push_back(shipSize);
}

/**
 * @brief Removes a ship from the shipLengths vector.
 * @param shipSize The length of the ship being removed.
 */
void HeatMap::deleteShip(int shipSize) {
	for(unsigned int i = 0; i < shipLengths.size(); i++) {
		if(shipSize == shipLengths.at(i)) {
			shipLengths.erase(shipLengths.begin() + i);
			return;
		}
	}
}

/**
 * @brief Prints the heatmap (used for debugging purposes).
 */
void HeatMap::printHeatMap() {
	for(int row = 0; row < boardSize; row++) {
		for(int col = 0; col < boardSize; col++) {
			cout << setw(3) << basicProbabilityMap[row][col] << " ";
		}
		cout << endl << flush;
	}
}

/**
 * @brief Clears all values in the heatmap to 0.
 */
void HeatMap::resetHeatMap() {
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			basicProbabilityMap[i][j] = 0;
		}
	}
}

/**
 * @brief Determines the best location to shoot.
 * @param rowToShoot The pointer of the optimal row index to shoot at (ByRef).
 * @param colToShoot The pointer of the optimal column index to shoot at (ByRef).
 */
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

/**
 * @brief Takes the board of the previous match and adds values to a heatmap that holds values for every time the opponent has placed a ship in a certain location.
 * @param playerBoard The board of the previous round
 */
void HeatMap::addPrevRoundData(char playerBoard[10][10]) {
	for(int row = 0; row < boardSize; row++) {
		for(int col = 0; col < boardSize; col++) {
			if(playerBoard[row][col] == HIT || playerBoard[row][col] == KILL) {
				longRunningMap[row][col] += 1;
			} else if(playerBoard[row][col] == MISS) {
				//longRunningMap[row][col] -= 1;
			}
		}
	}
}

/**
 * @brief Adds the heatmap of the opponent's ship placements to the heatmap of likely spots
 */
void HeatMap::addMapsTogether() {
	for(int row = 0; row < boardSize; row++) {
		for(int col = 0; col < boardSize; col++) {
			basicProbabilityMap[row][col] += longRunningMap[row][col];
		}
	}
}
