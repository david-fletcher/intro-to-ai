
/**
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004
 *
 * Please type in your name[s] here:
 * Jordan Wood, David Fletcher, Ryan Houck
 */

#ifndef Cheddar_H		// Double inclusion protection
#define Cheddar_H

using namespace std;

#include <vector>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

// Da Cheese inherits from/extends PlayerV2

class HeatMap {
	public:
		HeatMap() {};
		~HeatMap() {};
		void getShot(int& rowToShoot, int& colToShoot);
		void initializeHeatMap(int sizeOfBoard);
		void generateProbability(char playerBoard[10][10]);
		void addShip(int shipSize);
		void deleteShip(int shipSize);
		void printHeatMap();
		void printShipVector();
		void addPrevRoundData(char playerBoard[10][10]);
	private:
		void addMapsTogether();
		void resetHeatMap();
		int basicProbabilityMap[10][10];
		int longRunningMap[10][10];
			int boardSize;
			vector<int> shipLengths;
};

class ShipMap {
	public:
		ShipMap() {};
		~ShipMap() {};
		void initializeShipMap(int boardSz);
		void convertToShipMap(char board[10][10]);
		void bestShipLocation(int shipSize, int& row, int& col, int& direction);
		void resetCurShipPlacement();
	private:
		void placeShipOnMap(int r, int c, int dir, int shipSz);
			int map[10][10];
			int curShipPlacement[10][10];
			int boardSize;
};


class Cheddar: public PlayerV2 {
    public:
		Cheddar( int boardSize );
		~Cheddar();
		void newRound();
		Message placeShip(int length);
		Message getMove();
		void update(Message msg);

    private:
		enum ShotMode { SEEK, HUNT };
		enum TargetDirection { UP, DOWN, LEFT, RIGHT };
		void huntForKill(int& shotRow, int& shotCol, int lastHitR, int lastHitC, TargetDirection& direction);
		bool isValidMove(int row, int col);
		bool isValidPlacement(int length, int row, int col, int dir);
		void findShipLocation(int &row, int &col, int length, int dir);
		void updateCheddarBoard(int row, int col, int length, int dir);
		void searchForDamagedShips(int& nextRow, int& nextCol, ShotMode& sm);
		void initializeBoard();
			HeatMap heatMap;
			ShipMap shipMap;
			ShotMode shotmode;
			TargetDirection td;
			int lastRow, lastCol, row, col, lastHitRow, lastHitCol;
			int numShipsPlaced;
			int boardSize;
			int killSize;
			int gameCount;
			char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
			char opponentShots[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
			char cheddarBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
