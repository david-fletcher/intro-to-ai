/*
 * @brief Cheddar AI for battleships
 * @file Cheddar.cpp
 * @author Stefan Brandle, Jonathan Geisler, Jordan Wood, David Fletcher, Ryan Houck
 * @date September, 2004, Updated 2015 for multi-round play, last modified April 14, 2018 to make an improved AI
 *
 * This Battleships AI is the 2018 Spring code written by Jordan Wood,
 * David Fletcher, and Ryan Houck (based upon Dr. Brandle and Dr.
 * Geisler's base code that placed ships in the top left corner and
 * shot from left to right, then top to bottom).
 */

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
Cheddar::Cheddar( int boardSz ) :PlayerV2(boardSz) {
	boardSize = boardSz;
	heatMap.initializeHeatMap(boardSz);
	shipMap.initializeShipMap(boardSz);
	gameCount = 0;
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
Cheddar::~Cheddar( ) {}

/**
 * @brief: Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void Cheddar::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
		for(int col=0; col<boardSize; col++) 
		{
			this->opponentShots[row][col] = WATER;
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
	if(killSize != 0){
		heatMap.deleteShip(killSize);
		killSize = 0;
	}
	if(shotmode == SEEK) {
		heatMap.generateProbability(board);
		heatMap.getShot(lastRow, lastCol);
		
		if(isValidMove(lastRow, lastCol)){
			Message result( SHOT, lastRow, lastCol, "Bang", None, 1 );
			return result;
		} else {
			return getMove();
		}
	} else if(shotmode == HUNT) {
		huntForKill(row, col, lastHitRow, lastHitCol, td);
		
		if(isValidMove(row, col)){
			Message result( SHOT, row, col, "Bang", None, 1 );
			return result;
		}
		else{
			shotmode = SEEK;
			return getMove();
		}
	} else {
		Message result(SHOT, 0, 0, "Whoops", None, 1);
		return result;
	}
}

/**
  * @brief Checks adjacent spots for a kill after getting a hit
  * @param col The pointer for the current column being looked at.
  * @param row The pointer for the current row being looked at.
  * @param lastHitR The row index of the last hit that was processed.
  * @param lastHitC The column index of the last hit that was processed.
  * @param direction The pointer for the direction that you are shooting (ByRef).
  */
void Cheddar::huntForKill( int& shotRow, int& shotCol, int lastHitR, int lastHitC, TargetDirection& direction) {
	switch( direction ) {
		case UP:   if( lastHitR > 0 ) { 
						if( board[lastHitR-1][lastHitC] == WATER ) {
							shotRow = lastHitR-1;
							shotCol = lastHitC;
						}
						else if( board[lastHitR-1][lastHitC] == HIT ) {
							huntForKill( shotRow, shotCol, lastHitR-1, lastHitC, direction );
							return;
						}
						else if( board[lastHitR-1][lastHitC] == MISS || board[lastHitR-1][lastHitC] == KILL ) {
							direction = RIGHT;
							huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
							return;
						}
					}
					else {
						direction = RIGHT;
						huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
						return;
					}
				   break;
		case DOWN: if( lastHitR < boardSize-1 ) { 
						if( board[lastHitR+1][lastHitC] == WATER ) { 
							shotRow = lastHitR+1;
							shotCol = lastHitC;
						}
						else if( board[lastHitR+1][lastHitC] == HIT ) {
							huntForKill( shotRow, shotCol, lastHitR+1, lastHitC, direction );
							return;
						}
						else if( board[lastHitR+1][lastHitC] == MISS || board[lastHitR+1][lastHitC] == KILL ) {
							direction = UP;
							huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
							return;
						}
					}
					else {
						direction = UP;
						huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
						return;
					}
					break;
		case LEFT:  if( lastHitC > 0 ) { 
						if( board[lastHitR][lastHitC-1] == WATER ) { 
							shotRow = lastHitR;
							shotCol = lastHitC-1;
						}
						else if( board[lastHitR][lastHitC-1] == HIT ) {
							huntForKill( shotRow, shotCol, lastHitR, lastHitC-1, direction );
							return;
						}
						else if( board[lastHitR][lastHitC-1] == MISS || board[lastHitR][lastHitC-1] == KILL ) {
							direction = DOWN;
							huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
							return;
						}
					}
					else {
						direction = DOWN;
						huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
						return;
					}
					break;
		case RIGHT: if( lastHitC < boardSize-1 ) { 
						if( board[lastHitR][lastHitC+1] == WATER ) {
							shotRow = lastHitR;
							shotCol = lastHitC+1;
						}
						else if( board[lastHitR][lastHitC+1] == HIT ) {
							huntForKill( shotRow, shotCol, lastHitR, lastHitC+1, direction );
							return;
						}
						else if( board[lastHitR][lastHitC+1] == MISS || board[lastHitR][lastHitC+1] == KILL ) {
							direction = LEFT;
							huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
							return;
						}
					}
					else {
						direction = LEFT;
						huntForKill( shotRow, shotCol, lastHitR, lastHitC, direction );
						return;
					}
					break;
		default: break;
	}
}

/**
 * @brief Checks if a move is valid.
 * @return True if the move is valid, false otherwise.
 */
bool Cheddar::isValidMove(int row, int col){
	return (board[row][col] == WATER);
}

/**
 * @brief Performs any inter-round initialization.
 */
void Cheddar::newRound() {
    this->lastRow = 0;
    this->lastCol = -1;
    this->lastHitRow = 0;
    this->lastHitCol = 0;
    this->numShipsPlaced = 0;

    this->initializeBoard();
	
	this->shotmode = SEEK; // begin by looking for ships
	this->td = (rand()%2 == 0)? RIGHT : DOWN;
	//this->td = RIGHT; // OLD td init
	this->row = 0;
	this->col = 0;

	this->gameCount++;

	shipMap.resetCurShipPlacement();
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

	heatMap.addShip(length);
	int row, col, dir;

	if(gameCount > 1 || boardSize < 10) {
			shipMap.bestShipLocation(length, row, col, dir);
	} else {
		dir = rand() % 2;
		findShipLocation(row, col, length, dir);
	}
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
}

/**
 * @brief Updates the ship placement board.
 * @param row The row index of the ship's starting location.
 * @param col The column index of the ship's starting location.
 * @param length The length of the ship.
 * @param dir An integer that holds the direction of the ship (0 = horizontal, 1 = vertical).
 */
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
 * @brief Randomly places ships on the board.
 * @param row A reference to the int that we'll set as the starting row location.
 * @param col A reference to the int that we'll set as the starting column location.
 * @param length The length of the ship to be placed.
 * @param dir An integer that holds the direction of the ship (0 = horizontal, 1 = vertical).
 */
void Cheddar::findShipLocation( int& row, int& col, int length, int dir ) { // 0 == Horizontal, 1 == Vertical
	if( dir == 1 ) {
		row = rand() % (boardSize - length + 1);
		col = rand() % boardSize;
		
		for( int r = row; r < row + length; r++ ) {
			if( cheddarBoard[r][col] == WATER ) { continue; } 
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}
	else { // dir == 0
		row = rand() % boardSize;
		col = rand() % (boardSize - length + 1);
		
		for( int c = col; c < col + length; c++ ) {
			if( cheddarBoard[row][c] == WATER ) { continue; } 
			else { findShipLocation( row, col, length, dir ); break; }
		}
	}

	return;	
}

bool Cheddar::isValidPlacement(int length, int row, int col, int dir){
	//Vertical
	if(dir == 1){
		for(int k = 0; k < length; k++){
			if(cheddarBoard[row+k][col] != WATER){
				return false;
			}
		}
	}

	//Horizontal
	else{
		for(int k = 0; k < length; k++){
			if(cheddarBoard[row][col+k] != WATER){
				return false;
			}
		}
	}

	return true;
}


/**
 * @brief Checks the board for damaged ships, and sets the shot mode to hunt if there are any hits (not kills).
 * @param nextRow The pointer for the row index of a hit (ByRef).
 * @param nextCol The pointer for the column index of a hit (ByRef).
 * @param sm The pointer for the shot mode (ByRef).
 */
void Cheddar::searchForDamagedShips(int& nextRow, int& nextCol, ShotMode& sm) {
	for(int r = 0; r < boardSize; r++) {
		for(int c = 0; c < boardSize; c++) {
			if(board[r][c] == HIT) {
				nextRow = r;
				nextCol = c;
				sm = HUNT;
				return;
			}
		}
	}

	sm = SEEK;
	return;
}

/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void Cheddar::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:  shotmode = HUNT;
			   board[msg.getRow()][msg.getCol()] = msg.getMessageType();
			   lastHitRow = msg.getRow();
			   lastHitCol = msg.getCol();
			   break;
	case KILL: shotmode = SEEK;
			   board[msg.getRow()][msg.getCol()] = msg.getMessageType();
			   searchForDamagedShips(lastHitRow, lastHitCol, shotmode);
			   killSize++;
			   break; 
	case MISS:
	    board[msg.getRow()][msg.getCol()] = msg.getMessageType();
	    break;
	case WIN:
	case LOSE:
	case TIE:
		shipMap.convertToShipMap(opponentShots);
		heatMap.addPrevRoundData(board);
	    break;
	case OPPONENT_SHOT:
		opponentShots[msg.getRow()][msg.getCol()] = MISS;
	    break;
    }
}

//HeatMap.cpp

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
						basicProbabilityMap[i+k][j] += 3;
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
						basicProbabilityMap[i][j+k] += 3;
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
	int maxIndex = -99999;
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
				longRunningMap[row][col] += 3;
			} else if(playerBoard[row][col] == MISS) {
				longRunningMap[row][col] -= 1;
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

//ShipMap.cpp

/**
 * @brief Initializes the ship map.
 * @param boardSz The size of the Battleships board.
 */
void ShipMap::initializeShipMap(int boardSz){
	boardSize = boardSz;
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			map[i][j] = map[0][0];
		}
	}
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
				tempShipMap[i][j] = 5; //Magic number
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
	int minShipVert = 0;
	int minShipHoriz = 0;
	for(int i = 0; i < shipSize; i++){
		minShipVert += curShipPlacement[i][0];
		minShipHoriz += curShipPlacement[0][i];
	}
	int minVertR = 0;
	int minVertC = 0;
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
			curShipPlacement[r+i][c] += 9999;
		}
	}
	else{
		for(int i = 0; i < shipSz; i++){
			curShipPlacement[r][c+i] += 9999;
		}
	}
}

