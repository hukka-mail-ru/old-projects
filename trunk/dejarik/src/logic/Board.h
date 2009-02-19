#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include "Cell.h"
#include "Macros.h"
#include "Piece.h"
#include "Player.h"

#define CIRCLES 2 
#define RADIUSES 12 // cells in a circle

CLASSPTR(Board);
CLASSPTR(Piece);
CLASSPTR(Player);
CLASSPTR(Cell);

class Board // only one board in game
{
    friend class TestBoard;
    friend class TestPlayer;
    
public:
    
    // on Game start  
    Board();
    
    void getInitialCells(std::vector<CellPtr>& cells);
    
    void placePiece(const PiecePtr& piece, unsigned c, unsigned r);
    void placePiece(const PiecePtr& piece, const CellPtr& cell);
    
    void distribute(const PiecePtr& piece, const PlayerPtr& player);
    
    // on Game over
    void clear();
    
    // on user click
    CellPtr& getCell(unsigned c, unsigned r);      
    void getCells(std::vector<CellPtr>& cells); // get all cells

    // on user click1: (activate a piece => show possible moves and targets)
    void getPossibleMoves(const PiecePtr& piece, std::vector<CellPtr>& moves);
    void getPossibleTargets(const PiecePtr& piece, std::vector<CellPtr>& targets);

    // on user click2: (move the piece OR attack a partner's piece OR push a piece)
    bool isMoveValid(unsigned c, unsigned r);
    void getMoveSteps(unsigned c, unsigned r, std::vector<CellPtr>& steps);
    
    // on a kill
    void killPiece(PiecePtr& piece);
    
    // on a push
    void getPossiblePushes(const PiecePtr& piece, std::vector<CellPtr>& pushes);
    const PiecePtr& getActivePiece();
    
private:
    
    void unmarkAll();
    
    enum WhatToMark
    {
        POSSIBLE_MOVES,
        POSSIBLE_TARGETS
    };
    
    void markNeibours(WhatToMark whatToMark, unsigned step, const CellPtr& cell);       
    void mark(WhatToMark whatToMark, unsigned step, const CellPtr& prev, const CellPtr& cell);
    
    unsigned getRightPos(unsigned pos);
    unsigned getLeftPos(unsigned pos);
    
    std::vector<CellPtr> mCells; // all the cells on board
    
    std::vector<CellPtr> mPossibleMoves;
    
    PiecePtr mActivePiece;
};


#endif /*BOARD_H_*/