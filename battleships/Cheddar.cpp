/**
 * @brief DumbPlayer AI for battleships
 * @file DumbPlayerV2.cpp
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004 Updated 2015 for multi-round play.
 *
 * This Battleships AI is very simple and does nothing beyond playing
 * a legal game. However, that makes it a good starting point for writing
 * a more sophisticated AI.
 *
 * The constructor
 */

#include <iostream>
#include <cstdio>

#include "Cheddar.h"


/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.
 */
Cheddar::Cheddar( int boardSz )
    :PlayerV2(boardSz)
{
	boardSize = boardSz;
    // Could do any initialization of inter-round data structures here.
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
Cheddar::~Cheddar( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void Cheddar::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
		for(int col=0; col<boardSize; col++) {
			this->board[row][col] = WATER;
			this->cheddarBoard[row][col] = WATER;
		}
    }
}


/**
 * @brief Specifies the AI's shot choice and returns the information to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 *
 * See the Message class documentation for more information on the 
 * Message constructor.
 */
Message Cheddar::getMove() {
	int col = -1;
	int row = -1;
	seekKill(&row, &col);
	if(col == -1){
		lastCol += 3;
		if( lastCol >= boardSize ) {
			lastCol = 0 + (lastRow % 3);
			lastRow++;
		}
		if( lastRow >= boardSize ) {
			lastCol = 0;
			lastRow = 0;
		}
		col = lastCol;
		row = lastRow;
	}

	if(isValidMove(row, col)){
		Message result( SHOT, row, col, "Bang", None, 1 );
		return result;
	}
	else{
		return getMove();
	}
}

/**
  * @brief Checks adjacent spots for a kill after getting a hit
  * @param col The pointer for the column
  * @param row The pointer for the row
  */
void Cheddar::seekKill(int *row, int *col){
	/*for(int r = 0; r < boardSize; r++){
		for(int c = 0; c < boardSize; c++){
			if(board[r][c] == 'X'){
				//If there is a surrounding hit spot:
				if((r > 0 && board[r-1][c] == '~') && (r < boardSize - 1  && board[r+1][c] == 'X')){ // UP
					*row = r - 1;
					*col = c;
				}
				else if((c > 0 && board[r][c-1] == '~') && (c < boardSize - 1  && board[r][c+1] == 'X')){ // LEFT
					*row = r;
					*col = c - 1;
				}
				else if((r < boardSize - 1 && board[r+1][c] == '~') && (r > 0  && board[r-1][c] == 'X')){ // DOWN
					*row = r + 1;
					*col = c;
				}
				else if((c < boardSize - 1 && board[r][c+1] == '~') && (c > 0  && board[r][c-1] == 'X')){ // RIGHT
					*row = r;
					*col = c + 1;
				}
				//If there is no other hit spot around:
				else if(r > 0 && board[r-1][c] == '~'){ // UP
					*row = r - 1;
					*col = c;
				}
				else if(c > 0 && board[r][c-1] == '~'){ // LEFT
					*row = r;
					*col = c - 1;
				}
				else if(r < boardSize - 1 && board[r+1][c] == '~'){ // DOWN
					*row = r + 1;
					*col = c;
				}
				else if(c < boardSize - 1 && board[r][c+1] == '~'){ // RIGHT
					*row = r;
					*col = c + 1;
				}
				return;
			}
		}
	}*/

	for(int r = 0; r < boardSize; r++){
		for(int c = 0; c < boardSize; c++){
			if(board[r][c] == HIT){
				if( r > 0 && r < boardSize - 1 && board[r-1][c] == HIT && board[r+1][c] == WATER) { // if hit below, shoot above
					*row = r - 1;
					*col = c;
				}
				else if( r > 0 && r < boardSize - 1 && board[r+1][c] == HIT && board[r-1][c] == WATER) {
					*row = r + 1;
					*col = c;
				}
				else if( c > 0 && c < boardSize - 1 && board[r][c-1] == HIT && board[r][c+1] == WATER) {
					*row = r;
					*col = c - 1;
				}
				else if( c > 0 && c < boardSize - 1 && board[r][c+1] == HIT && board[r][c-1] == WATER) {
					*row = r;
					*col = c + 1;
				}
				else if(r > 0 && board[r-1][c] == WATER){ // UP

					*row = r - 1;
					*col = c;
				}
				else if(c > 0 && board[r][c-1] == WATER){ // LEFT
					*row = r;
					*col = c - 1;
				}
				else if(r < boardSize - 1 && board[r+1][c] == WATER){ // DOWN
					*row = r + 1;
					*col = c;
				}
				else if(c < boardSize - 1 && board[r][c+1] == WATER){ // RIGHT
					*row = r;
					*col = c + 1;
				}
				return;
			}
		}
	}

}

bool Cheddar::isValidMove(int row, int col){
	return board[row][col] == '~';
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void Cheddar::newRound() {
    /* DumbPlayer is too simple to do any inter-round learning. Smarter players 
     * reinitialize any round-specific data structures here.
     */
    this->lastRow = 0;
    this->lastCol = -1;
    this->numShipsPlaced = 0;

    this->initializeBoard();
}

/**
 * @brief Gets the AI's ship placement choice. This is then returned to the caller.
 * @param length The length of the ship to be placed.
 * @return Message The most important parts of the returned message are 
 * the direction, row, and column values. 
 *
 * The parameters returned via the message are:
 * 1. the operation: must be PLACE_SHIP 
 * 2. ship top row value
 * 3. ship top col value
 * 4. a string for the ship name
 * 5. direction Horizontal/Vertical (see defines.h)
 * 6. ship length (should match the length passed to placeShip)
 */
Message Cheddar::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

	int row, col, dir;

	dir = rand() % 2;

	findShipLocation(row, col, length, dir);

	updateCheddarBoard(row, col, length, dir);

	if( dir == 0 ) {
		Message response( PLACE_SHIP, row, col, shipName, Horizontal, length );
		numShipsPlaced++;

		return response;
	} else {
		Message response( PLACE_SHIP, row, col, shipName, Vertical, length );
		numShipsPlaced++;

		return response;
	}

    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
}

void Cheddar::findShipLocation(int &row, int &col, int length, int dir) {

	if( dir == 1 ) {
		row = rand() % (boardSize - length + 1);
		col = rand() % boardSize;

		for( int r = row; r < row + length; r++ ) {
			if( cheddarBoard[r][col] == '~' ) { continue; }
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}
	else {	
		col = rand() % (boardSize - length + 1);
		row = rand() % boardSize;

		for( int c = col; c < col + length; c++ ) {
			if( cheddarBoard[row][c] == '~' ) { continue; }
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}

}

void Cheddar::updateCheddarBoard(int row, int col, int length, int dir) {
	if(dir == 0) {
		for( int c = col; c < col + length; c++ ) {
			cheddarBoard[row][c] = SHIP;
		}
	} else {
		for (int r = row; r < row + length; r++ ) {
			cheddarBoard[r][col] = SHIP;
		}
	}
}

/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void Cheddar::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
	case KILL:
	case MISS:
	    board[msg.getRow()][msg.getCol()] = msg.getMessageType();
	    break;
	case WIN:
	    break;
	case LOSE:
	    break;
	case TIE:
	    break;
	case OPPONENT_SHOT:
	    // TODO: get rid of the cout, but replace in your AI with code that does something
	    // useful with the information about where the opponent is shooting.
	    //cout << gotoRowCol(20, 30) << "DumbPl: opponent shot at "<< msg.getRow() << ", " << msg.getCol() << flush;
	    break;
    }
}

