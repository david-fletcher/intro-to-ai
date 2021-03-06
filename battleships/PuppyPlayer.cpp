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
 * THIS FILE HAS BEEN MODIFIED BY: David Fletcher and Anna Cummings
 * This is our attempt to smarten the DumbPlayer class
 *
 * The constructor
 */

#include "PuppyPlayer.h"

using namespace conio;

/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.
 */
PuppyPlayer::PuppyPlayer( int boardSize )
    :PlayerV2(boardSize)
{
	// Could do any initialization of inter-round data structures here.
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
PuppyPlayer::~PuppyPlayer( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void PuppyPlayer::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
		for(int col=0; col<boardSize; col++) {
	    	this->puppyBoard[row][col] = WATER;
			this->board[row][col] = WATER;
		}
    }
}


/**
 * @brief Specifies the AI's shot choice and returns the information to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 *
 * THIS FUNCTION HAS BEEN MODIFIED BY: David Fletcher and Anna Cummings
 * We changed the shot behavior to make the DumbPlayer smarter
 *
 * See the Message class documentation for more information on the 
 * Message constructor.
 */
Message PuppyPlayer::getMove() {
    getShot();

	Message result( SHOT, lastRow, lastCol, "Bang", None, 1 );
    return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void PuppyPlayer::newRound() {
    /* DumbPlayer is too simple to do any inter-round learning. Smarter players 
     * reinitialize any round-specific data structures here.
     */
    this->lastRow = -1;
    this->lastCol = -1;
	this->lastHitRow = 0;
	this->lastHitCol = 0;
    this->numShipsPlaced = 0;

	this->shotstrats = HUNT;
	this->tarDir = RIGHT;

	this->populateHuntArray( boardSize );
    this->initializeBoard();

	calculateProbabilities( huntArray );
	sort( huntArray.begin(), huntArray.end(), locCompare );
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
Message PuppyPlayer::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);
	int row, column, dir;

	dir = rand() % 2; // decides direction; 0 == Horizontal, 1 == Vertical

	findShipLocation(row, column, length, dir);

	updatePuppyBoard(row, column, length, dir);

    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
    if( dir == 0 ) {
		Message response( PLACE_SHIP, row, column, shipName, Horizontal, length );
		numShipsPlaced++;
    	return response;
    } else {
		Message response( PLACE_SHIP, row, column, shipName, Vertical, length );
		numShipsPlaced++;
    	return response;
	}
}

/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void PuppyPlayer::update(Message msg) {
	switch(msg.getMessageType()) {
	case HIT:  shotstrats = TARGET; 
			   board[msg.getRow()][msg.getCol()] = msg.getMessageType(); 
			   lastHitRow = msg.getRow();
			   lastHitCol = msg.getCol();
			   break;
	case KILL: board[msg.getRow()][msg.getCol()] = msg.getMessageType();
			   //tarDir = RIGHT;
			   checkForDamagedShips( lastHitRow, lastHitCol, tarDir, shotstrats );
			   break;
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
	    // cout << gotoRowCol(20, 30) << "DumbPl: opponent shot at "<< msg.getRow() << ", " << msg.getCol() << flush;
	    break;
    }

	calculateProbabilities( huntArray );
	sort( huntArray.begin(), huntArray.end(), locCompare );
}

/**
 * @brief Searches the board for hit ships and seeks to kill them
 * @param shotRow a local variable that is given a value so the getMove() function knows where to shoot
 * @param shotCol a local variable that is given a value so the getMove() function knows where to shoot
 */
void PuppyPlayer::targetNext( int& shotRow, int& shotCol, int lastHitR, int lastHitC, TargetDirection& direction, ShotStrategy& shotstrats ) {
	switch( direction ) {
		case UP:   if( lastHitR > 0 ) { 
						if( board[lastHitR-1][lastHitC] == WATER ) {
							shotRow = lastHitR-1;
							shotCol = lastHitC;
						}
						else if( board[lastHitR-1][lastHitC] == HIT ) {
							targetNext( shotRow, shotCol, lastHitR-1, lastHitC, direction, shotstrats );
							return;
						}
						else if( board[lastHitR-1][lastHitC] == MISS || board[lastHitR-1][lastHitC] == KILL ) {
							direction = RIGHT;
							targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
							return;
						}
					}
					else {
						direction = RIGHT;
						targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats );
						return;
					}
				   break;
		case DOWN: if( lastHitR < boardSize-1 ) { 
						if( board[lastHitR+1][lastHitC] == WATER ) { 
							shotRow = lastHitR+1;
							shotCol = lastHitC;
						}
						else if( board[lastHitR+1][lastHitC] == HIT ) {
							targetNext( shotRow, shotCol, lastHitR+1, lastHitC, direction, shotstrats  );
							return;
						}
						else if( board[lastHitR+1][lastHitC] == MISS || board[lastHitR+1][lastHitC] == KILL ) {
							direction = UP;
							targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
							return;
						}
					}
					else {
						direction = UP;
						targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
						return;
					}
					break;
		case LEFT:  if( lastHitC > 0 ) { 
						if( board[lastHitR][lastHitC-1] == WATER ) { 
							shotRow = lastHitR;
							shotCol = lastHitC-1;
						}
						else if( board[lastHitR][lastHitC-1] == HIT ) {
							targetNext( shotRow, shotCol, lastHitR, lastHitC-1, direction, shotstrats  );
							return;
						}
						else if( board[lastHitR][lastHitC-1] == MISS || board[lastHitR][lastHitC-1] == KILL ) {
							direction = DOWN;
							targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
							return;
						}
					}
					else {
						direction = DOWN;
						targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
						return;
					}
					break;
		case RIGHT: if( lastHitC < boardSize-1 ) { 
						if( board[lastHitR][lastHitC+1] == WATER ) {
							shotRow = lastHitR;
							shotCol = lastHitC+1;
						}
						else if( board[lastHitR][lastHitC+1] == HIT ) {
							targetNext( shotRow, shotCol, lastHitR, lastHitC+1, direction, shotstrats  );
							return;
						}
						else if( board[lastHitR][lastHitC+1] == MISS || board[lastHitR][lastHitC+1] == KILL ) {
							direction = LEFT;
							targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
							return;
						}
					}
					else {
						direction = LEFT;
						targetNext( shotRow, shotCol, lastHitR, lastHitC, direction, shotstrats  );
						return;
					}
					break;
		default: break;
	}
}

void PuppyPlayer::findShipLocation( int& row, int& col, int length, int dir ) { // 0 == Horizontal, 1 == Vertical
	if( dir == 1 ) {
		row = rand() % (boardSize - length + 1);
		col = rand() % boardSize;
		
		for( int r = row; r < row + length; r++ ) {
			if( puppyBoard[r][col] == WATER ) { continue; } 
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}
	else { // dir == 0
		row = rand() % boardSize;
		col = rand() % (boardSize - length + 1);
		
		for( int c = col; c < col + length; c++ ) {
			if( puppyBoard[row][c] == WATER ) { continue; } 
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}

	return;	
}

void PuppyPlayer::updatePuppyBoard( int row, int col, int length, int dir ) {
	if( dir == 0 ) {	
		for( int c = col; c < col + length; c++ ) {
			puppyBoard[row][c] = SHIP;
		}
	}
	else { //dir == 1
		for( int r = row; r < row + length; r++ ) {
			puppyBoard[r][col] = SHIP;
		}
	}
}

void PuppyPlayer::populateHuntArray( int boardSize ) {
	huntArray.clear();
	for( int row = 0; row < boardSize; row++ ) {
		for( int col = row % 3; col < boardSize; col+=3 ) {
			if ( col >= boardSize ) {
				break;
			}
			else {
				struct Location tempLoc;
				tempLoc.row = row;
				tempLoc.col = col;
				tempLoc.probability = 0;
				huntArray.push_back(tempLoc);
			}
		}
	}
}

void PuppyPlayer::checkForDamagedShips( int& lastHitR, int& lastHitC, TargetDirection& direction, ShotStrategy& shotstrats ) {

	for( int r = 0; r < boardSize; r++ ) {
		for( int c = 0; c < boardSize; c++ ) {
			if( board[r][c] == HIT ) {
				lastHitR = r;
				lastHitC = c;
				shotstrats = TARGET;
				return;
			}
		}
	}

	shotstrats = HUNT;
}

void PuppyPlayer::getShot() {
	switch( shotstrats ) {	
		case HUNT:
			if ( ! huntArray.empty() && board[huntArray.front().row][huntArray.front().col] == WATER ) {
				lastRow = huntArray.front().row;
				lastCol = huntArray.front().col;
				huntArray.erase( huntArray.begin() );
			}
			else {
				huntArray.erase( huntArray.begin() );
				getShot();
			}
			break;
		case TARGET:
			targetNext(lastRow, lastCol, lastHitRow, lastHitCol, tarDir /* sauce */, shotstrats );
			break;
		default: 
			break; // this should never be reached
	}
	
}

void PuppyPlayer::calculateProbabilities( vector<Location>& huntArray ) {
	int numSqUp, numSqDn, numSqLf, numSqRt;
	for( unsigned int i = 0; i < huntArray.size(); i++ ) {
		numSqUp = numSqDn = numSqLf = numSqRt = 0;

		//calculate how many water spaces are above any given index in the hunt array
		for( int up = 0; up < 5; up++ ) {
			if( huntArray.at(i).row - up >= 0 ) {	
				if( board[huntArray.at(i).row-up][huntArray.at(i).col] == WATER ) {
					numSqUp++;
				}
				else { break; }
			}
			else { break; }
		}
		//calculate the same for down
		for( int dn = 0; dn < 5; dn++ ) {
			if( huntArray.at(i).row + dn < boardSize ) {	
				if( board[huntArray.at(i).row+dn][huntArray.at(i).col] == WATER ) {
					numSqDn++;
				}
				else { break; }
			}
			else { break; }
		}
		//calculate the same for left
		for( int lf = 0; lf < 5; lf++ ) {
			if( huntArray.at(i).col - lf >= 0 ) {	
				if( board[huntArray.at(i).row][huntArray.at(i).col-lf] == WATER ) {
					numSqLf++;
				}
				else { break; }
			}
			else { break; }
		}
		//calculate the same for right
		for( int rt = 0; rt < 5; rt++ ) {
			if( huntArray.at(i).col + rt < boardSize ) {	
				if( board[huntArray.at(i).row][huntArray.at(i).col+rt] == WATER ) {
					numSqRt++;
				}
				else { break; }
			}
			else { break; }
		}

		int sumAll = 0;

		if( numSqUp + numSqDn > 3 ) {
			sumAll += numSqUp + numSqDn;
		}
		if( numSqLf + numSqRt > 3 ) {
			sumAll += numSqLf + numSqRt;
		}
		huntArray.at( i ).probability = sumAll;

	}
}
