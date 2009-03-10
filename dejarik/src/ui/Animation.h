#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Piece.h"
#include "Cell.h"
#include "Game.h"


class Animation
{
    friend class TestAnimation;
    Animation() {}; // for test
public:
    
    Animation(const GamePtr& game): mGame(game), mMoving(false) {}
    
    // retrun true if moving in process 
    bool updateAll(const std::vector<CellPtr>& moveSteps);
    
private:
  
    float getNormalAngle(float x, float y); // must be changed smoothly (when moving along orbit)
    float getTargetAngle(unsigned step);
    float getRotation(unsigned step);
    float shorterAngle(float ang);
    float getSmallestAngle(float start, float end); 
    void  updatePieceImage(const PiecePtr& piece);
    
    void initPiece(const PiecePtr& piece);
    void updatePiece(const PiecePtr& piece);
    
    GamePtr mGame;
    std::vector<CellPtr> mMoveSteps; 
    bool mMoving; // piece moving in progress
};


#endif /*ANIMATION_H_*/
