#ifndef GAME_H_
#define GAME_H_

#include "Macros.h"
#include "Board.h"
#include "Player.h"

CLASSPTR(Game);


// tests--------------------------------
extern bool TestPiecesMoveOneCell;
// --------------------------------

class Game
{
    friend class TestGame;
    
public:
    
    void start();
    
    bool isOver();

    /*
     * Defines if the mouse click is valid (true/false).
     * and transforms the mouse coordinates into cell coordinates 
     */
    bool isCell(unsigned x, unsigned y, unsigned& cellC, unsigned& cellX);
    
    /*
     * Reurns true if the click is allowed by the game rules 
     */
    bool onCellClick(unsigned c, unsigned x);

    
private:
        
    bool checkVictory(PlayerPtr& vinner);
    
    BoardPtr mBoard;
    
    PlayerPtr mPlayer1;
    PlayerPtr mPlayer2;
    
};



#endif /*GAME_H_*/
