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

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"
#include "conio.h"
#include "HeatMap.h"
#include "ShipMap.h"

// Da Cheese inherits from/extends PlayerV2

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
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
		char opponentShots[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
		char cheddarBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
