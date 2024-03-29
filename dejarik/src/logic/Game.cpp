#include <vector>
#include <sstream>

#include "Game.h"

#ifdef WIN_BUILD
#include <time.h>
#endif

#define NUM_MOVES 2

using namespace std;

void Game::startup()
{
    TRY_BEGINS;
    
    mBoard = BoardPtr(new Board());
    
    // TODO ask for player name
    
    mPlayer1 = PlayerPtr(new Player("Player1", mBoard));
    mPlayer2 = PlayerPtr(new Player("Player2", mBoard));

    mBoard->createPieces();
    vector<PiecePtr> pieces = mBoard->getPieces();
    
    vector<CellPtr> cells;
    mBoard->getInitialCells(cells);  
    const unsigned pieces_num = pieces.size();
           
    // randomly divide the pieces between the players, 
    // and place them on opposites sides of the board
    srand((unsigned)time(0));     
    
    for(unsigned i=0; i<pieces_num; i++)
    {
        unsigned rnd = (rand()%pieces.size()); // random 0.. pieces.size() - 1
        
        PlayerPtr player = (i < pieces_num/2) ? mPlayer1 : mPlayer2; 
        
        mBoard->placePiece(pieces[rnd], cells[i]);               
        mBoard->distribute(pieces[rnd], player);  
        
        pieces[rnd]->cellBeforeMoving = cells[i]; // zeroize cellBeforeMoving
        
        pieces.erase(remove(pieces.begin(), pieces.end(), pieces[rnd]), pieces.end()); 
    }
    
    passTurn();
    
    
    TRY_RETHROW;
}


void Game::passTurn(BattleResult battleResult)
{
    TRY_BEGINS;
    
    if(!mActivePlayer)
    {
        mActivePlayer = mPlayer1;
    }
    else
    {    
        mActivePlayer = (mActivePlayer == mPlayer1) ? mPlayer2 : mPlayer1; // next player
    }
    
    // no active piece at the beginning
    // but if RES_COUNTER_PUSH the player (enemy) must have an active piece (ours)
    if(battleResult != RES_COUNTER_PUSH)
    {
        mActivePlayer->resetActivePiece();
    }
    
    if(mActivePlayer->getLeftMoves() == 0)
    {
        mActivePlayer->setLeftMoves(NUM_MOVES);
        mBoard->definePossibleClicks(mActivePlayer);
    }
    
    
    
    TRY_RETHROW;
}



    
BattleResult Game::onCellClick(const CellPtr& cell)
{
    TRY_BEGINS;
    
    BattleResult res = RES_CLICK; 
    static BattleResult prevBattleResult = RES_CLICK;
    mBoard->deselectAll();
    
    if(!mBoard->isClickValid(cell))
    {
        mActivePlayer->resetActivePiece();
        return res;
    }
    
    // clicked on empty cell
    if(!cell->piece) 
    {
        if(!mActivePlayer->getActivePiece())
        {
            return res;
        }
        
        // common move
        mActivePlayer->movePiece(cell);      
        mActivePlayer->decrementLeftMoves();
        res = RES_MOVE;
        
        // push: we need to redefine where our pieces are.
        if(prevBattleResult == RES_PUSH)
        {
            mActivePlayer->resetActivePiece();
            mBoard->definePossibleClicks(mActivePlayer);
        }
    }
    else 
    {
        // clicked on our piece
        if(cell->piece->player == mActivePlayer) 
        {
            mActivePlayer->setActivePiece(cell->piece);
            mBoard->definePossibleClicks(mActivePlayer);
            mBoard->selectClickedCell(cell);
            
            cell->piece->cellBeforeMoving = cell;
        }
        
        // clicked on enemy's piece
        else 
        {
            PiecePtr myPiece = mActivePlayer->getActivePiece(); // "me" means Player1
            PiecePtr enemyPiece = cell->piece;                  // "enemy" means Player2
            
            res = mActivePlayer->attackEnimy(enemyPiece);
            
            if(res == RES_KILL)
            {
                mBoard->killPiece(const_cast<PiecePtr&>(enemyPiece));
                mActivePlayer->decrementLeftMoves();
                mActivePlayer->resetActivePiece();
                mBoard->definePossibleClicks(mActivePlayer);
            }
            else if(res == RES_COUNTER_KILL)
            {
                mBoard->killPiece(myPiece);
                mActivePlayer->decrementLeftMoves();
                mActivePlayer->resetActivePiece();
                mBoard->definePossibleClicks(mActivePlayer);
            }
            else if(res == RES_PUSH)
            {
                mBoard->definePossiblePushClicks(enemyPiece);                 
                mActivePlayer->setActivePiece(enemyPiece); // We now owns enemy's piece!
            }
            else if(res == RES_COUNTER_PUSH)
            {
                mActivePlayer->decrementLeftMoves();
                
                PlayerPtr enemy = (mActivePlayer == mPlayer1) ? mPlayer2 : mPlayer1; // next player
                mBoard->definePossiblePushClicks(myPiece);
                enemy->setLeftMoves(1);
                
                // enemy now owns our piece!
                enemy->setActivePiece(myPiece);
                
                passTurn(RES_COUNTER_PUSH);
                
                prevBattleResult = res;
                return res;
            }
        }
    }
    
    if(mActivePlayer->getLeftMoves() == 0)
    {
        PlayerPtr enemy = (mActivePlayer == mPlayer1) ? mPlayer2 : mPlayer1; // next player
        
        // special case: keep on turning if the 'COUNTER PUSH' was the result 
        // of the last enemy turn
        if(enemy->getLeftMoves() == 0 && prevBattleResult == RES_COUNTER_PUSH)
        {
            mActivePlayer->setLeftMoves(NUM_MOVES);
            mActivePlayer->resetActivePiece();
            mBoard->definePossibleClicks(mActivePlayer);
        }
        else // normal case
        {
            passTurn();
        }
    }

    prevBattleResult = res;
    return res;
    
    TRY_RETHROW;
    
}
    
bool Game::checkVictory(PlayerPtr& vinner)
{
    TRY_BEGINS;
    
    if(mPlayer1->howManyPieces() == 0)
    {
        vinner = mPlayer2;
        return true;
    }
    else if(mPlayer2->howManyPieces() == 0)
    {
        vinner = mPlayer1;
        return true;
    }
    
    TRY_RETHROW;
    
    return false;
}

const BoardPtr& Game::getBoard()
{
    return mBoard;
}

const Player* Game::getPlayer1()
{
    return mPlayer1.get();
}

const Player* Game::getPlayer2()
{
    return mPlayer2.get();
}

const Player* Game::getActivePlayer()
{
    return mActivePlayer.get();
}


