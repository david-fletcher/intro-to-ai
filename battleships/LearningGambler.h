/**
 * @authors Stefan Brandle & Jonathan Geisler
 * @date May, 2004
 *
 */

#ifndef LEARNINGGAMBLER_H
#define LEARNINGGAMBLER_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

class LearningGambler: public PlayerV2 {
  public:
    LearningGambler( int boardSize );
    void newRound();
    Message placeShip(int length);
    Message getMove();
    void update(Message msg);

  private:
    void getShot(int shot[]);
    void getNextProbe( int shot[] );
    void getFollowUpShot( int row, int col, int shot[] );
    bool search( int row, int col, int rowDelta, int colDelta, int shot[], int rangeLeft );
    void chooseValues( int& row, int& col, int& length, Direction& dir );
    bool positionOk( int row, int col, int length, Direction dir );
    void markShip( int row, int col, int length, Direction dir );
    bool haveUnfinishedBusiness( int position[2] );
    void reWeighBoard( int board[][MAX_BOARD_SIZE] );
    void printWeightedBoard( int board[][MAX_BOARD_SIZE] );
    void getMaxWeightCell( int board[][MAX_BOARD_SIZE], int shot[2] );
    int calcCellWeight( int row, int col );
    int calcCellWeightVert( int row, int col, int length );
    int calcCellWeightHoriz( int row, int col, int length );
    bool onBoard( int row, int col );
    char getCellStatus( int row, int col );
    void initializeBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);

    // Note: making assumptions about ship size.
    const int SHIP_MAX_LENGTH = MAX_SHIP_SIZE;
    const int SHIP_MIN_LENGTH = MIN_SHIP_SIZE;
    const float DECAY_RATE = 0.90;
    const int HISTORY_WEIGHT = 200;

    int lastRow, lastCol;
    int shot[2];
    char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int cellWeights[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int opponentShots[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    float opponentShips[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    bool shotBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int roundNumber;      // Round within contest
    int turnNumber;       // Turn within round -- count down from boardSize * boardSize

    // Ship placement items
    Message placeEdgePlusRandom(int length);
    int numShipsPlaced;
    char shipBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    char shipMark;
    int placementStrategy;

};

#endif
