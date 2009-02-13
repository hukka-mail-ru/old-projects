#include "Player.h"

#include <cstdlib> 
#include <ctime> 
#include <sstream>

using namespace std;


void Player::addPiece(const PiecePtr& piece)
{
    mPieces.push_back(piece);
}

void Player::removePiece(const PiecePtr& piece)
{
    unsigned init = mPieces.size();

    mPieces.erase(remove(mPieces.begin(), mPieces.end(), piece), mPieces.end());
    
    assert(mPieces.size() == init - 1);
}

unsigned Player::howManyPieces()
{
    return mPieces.size();
}


bool Player::makeTurn(unsigned c, unsigned x, TurnStage turnStage, BattleResult& battleResult) 
{
    TRY_BEGINS;
    
    // Define the cell: empty ? ally ? enemy ?
    CellPtr cell = mBoard->getCell(c, x);
    PiecePtr piece = cell->piece;
    
    // can't start from an empty cell
    if(!piece && turnStage == TURN_START)
    {
        return false;
    }
    
    // can't start from an enemy piece
    if(piece && piece->getPlayer().get() != this && turnStage == TURN_START)
    {
        return false;
    }
    
    // if clicked on ally, we must do TURN_START, even if we obtained TURN_FINISH
    if(piece && piece->getPlayer().get() == this && turnStage == TURN_FINISH)
    {
        turnStage = TURN_START;
    }
    
    // on TURN_START:
    if(turnStage == TURN_START)
    {
        mActivePiece = piece;
        
        vector<CellPtr> possibleMoves;
        mBoard->getPossibleMoves(piece, possibleMoves);
    }
    else if(turnStage == TURN_FINISH)
    {
        if(!piece) // move to cell
        {
            return moveActivePiece(c, x);
        }
        else // attack enemy piece
        {
            battleResult = attackEnimy(mActivePiece, piece); // here 'piece' definitely means 'enemy piece'
            return true;
        }
    }
    
    TRY_RETHROW;
    
    return true;

}


BattleResult Player::attackEnimy(const PiecePtr& myPiece, const PiecePtr& enemyPiece)
{
    TRY_BEGINS;
    
    // an assurance
    assert(myPiece->getPlayer() != enemyPiece->getPlayer());
    
    BattleResult res = getBattleResult(myPiece->getAttackRating(), enemyPiece->getDefenceRating());
    
    if(res == RES_KILL)
    {
        mBoard->killPiece(const_cast<PiecePtr&>(enemyPiece));
    }
    else if(res == RES_COUNTER_KILL)
    {
        mBoard->killPiece(const_cast<PiecePtr&>(myPiece));
    }
    
    return res;
    
    TRY_RETHROW;
}



BattleResult Player::getBattleResult(unsigned attackRating, unsigned defenceRating)
{
    srand((unsigned)time(0)); 
    
    unsigned attack = 0;
    for(unsigned i = 0; i < attackRating; i++)
    {
        attack += (rand()%6)+1; // random number 1..6 (like a dice) 
    }
    
    unsigned defence = 0;
    for(unsigned i = 0; i < defenceRating; i++)
    {
        defence += (rand()%6)+1; // random number 1..6 (like a dice) 
    }
    
    
    if(attack > defence)
    {
        if(attack >= defence + 7) // if Attack beats Defense by 7 or more then Kill
        {
            return RES_KILL;
        }
        else if(attack <= defence + 6) // if Attack beats Defense by 6 or less then Push
        {
            return RES_PUSH;
        }
    }
    else if (attack == defence) //  if a draw then Counter-Push
    {
        return RES_COUNTER_PUSH;
    }
    else if(defence > attack)
    {
        if(defence >= attack + 7) // if Defense beats attack by 7 or less then Counter-Kill
        {
            return RES_COUNTER_KILL;
        }
        else if(defence <= attack + 6) //  if Defense beats Attack by 6 or less then Counter-Push
        {
            return RES_COUNTER_PUSH;
        }
    }
    
    stringstream err;
    err << __FUNCTION__ << " Battle result is undefined: attack=" << attack << ", defence=" << defence;
    throw(err.str());
    
    return RES_NO_BATTLE;
}


bool Player::moveActivePiece(unsigned c, unsigned x) 
{
    TRY_BEGINS;
    
    if(!mBoard->isMoveValid(c, x))
    {
        return false;
    }
    
    vector<CellPtr> steps;
    mBoard->getMoveSteps(c, x, steps);
    
    for(unsigned i = 0; i<steps.size(); i++)
    {
        mBoard->placePiece(mActivePiece, steps[i]);
        cout << "moveActivePiece (" << mActivePiece->getName() << ")to " <<  steps[i]->c << "." << steps[i]->x << endl;
    }
    
    TRY_RETHROW;
    
    return true;
}
