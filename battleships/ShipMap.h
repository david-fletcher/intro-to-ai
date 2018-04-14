#ifndef ShipMap_H		// Double inclusion protection
#define ShipMap_H

#include <iostream>
#include <vector>
#include "defines.h"

using namespace std;

class ShipMap {
	public:
		ShipMap();
		~ShipMap() {};
		void initializeShipMap(int boardSz);
		void convertToShipMap(char board[10][10]);
		void bestShipLocation(int shipSize, int& row, int& col, int& direction);
		void resetCurShipPlacement();
	private:
		int map[10][10];
		int curShipPlacement[10][10];
		int boardSize;
		
		void placeShipOnMap(int r, int c, int dir, int shipSz);
};

#endif
