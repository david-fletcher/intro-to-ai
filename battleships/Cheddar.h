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

// DumbPlayer inherits from/extends PlayerV2

class Cheddar: public PlayerV2 {
    public:
	Cheddar( int boardSize );
	~Cheddar();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);

    private:
	void seekKill(int *row, int *col);
	bool isValidMove(int row, int col);
	void findShipLocation(int &row, int &col, int length, int dir);
	void updateCheddarBoard(int row, int col, int length, int dir);
	int boardSize;
	void initializeBoard();
        int lastRow;
        int lastCol;
	int numShipsPlaced;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
		char cheddarBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
