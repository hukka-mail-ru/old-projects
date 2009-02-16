#ifndef CELL_H_
#define CELL_H_

#include "Macros.h"
#include "Piece.h"

CLASSPTR(Cell);
CLASSPTR(Piece);

struct Cell
{
public:
    Cell(unsigned circle, unsigned xnum):
        c(circle),
        x(xnum),
        mark(0) // not marked initially
    {
        prev.reset();
        piece.reset();
    }
    
    // operator = needed 
    
    unsigned c; // coordinate: 0,1,2
    unsigned x; // coordinate: 0 in circle 0;   0..11 in circle 1;  0..11 in circle 2.  
    
    unsigned mark;
    CellPtr prev;
    
    PiecePtr piece;
};



#endif /*CELL_H_*/
