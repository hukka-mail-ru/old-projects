#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>
#include "Macros.h"
#include "Piece.h"
#include "Board.h"

CLASSPTR(Piece);
CLASSPTR(Board);
CLASSPTR(Player);

#ifdef UNIT_TESTS
extern bool TestMakePush;
extern bool TestMakeCounterPush;
#endif

// e.g. a move for 1.0 to 2.0
// =>  TURN_SELECTION at 1.0, TURN_ACTION at 2.0
enum TurnStage 
{
    TURN_SELECTION, 
    TURN_ACTION
};

enum BattleResult
{
    RES_NO_BATTLE,
    RES_KILL,
    RES_PUSH,
    RES_COUNTER_KILL,
    RES_COUNTER_PUSH
};

class Player
{
    friend class TestPlayer;
    friend class TestGame;
    
public:
    Player(const std::string& name, const BoardPtr& board): 
        mName(name),
        mBoard(board) 
        {}
    
    void addPiece(const PiecePtr& piece);
    
    void removePiece(const PiecePtr& piece);
    
    unsigned howManyPieces();
    
    /* 
     Returns true if the move is valid, else false

     Define the cell: empty ? ally ? enemy ?
      
    if clicked on ally, we must do TURN_SELECTION, even we are obtained TURN_ACTION
    
    on TURN_SELECTION:
       select mActivePiece
       player gets possible moves 
    
    on TURN_ACTION:
      player moves mActivePiece 
      or attacks an enemy piece:
      
      battleResult: output parameter
    */
    bool makeTurn(unsigned c, unsigned x, TurnStage turnStage, BattleResult& battleResult);
    
    /*
     * TURN_SELECTION: automatically when Game defined RES_PUSH or RES_COUNTER_PUSH
     *    definePossiblePushes -> on the base of definePossibleMoves
     *    show them on screen
     * 
     * TURN_ACTION: by player
     *    isMoveValid
     *    move enemy piece
     */
    bool makePush(unsigned c, unsigned x);
    
private:
    
    bool movePiece(const PiecePtr& piece, unsigned c, unsigned x);    
    
    BattleResult attackEnimy(const PiecePtr& myPiece, const PiecePtr& enemyPiece);
    
    /*
        if Attack beats Defense by 7 or more then Kill
        if Attack beats Defense by 6 or less then Push
        if a draw then Counter-Push
        if Defense beats Attack by 6 or less then Counter-Push
        if Defense beats attack by 7 or less then Counter-Kill
    */
    BattleResult getBattleResult(unsigned attack, unsigned defence);
       
    std::string mName;
    
    BoardPtr mBoard;
    
    std::vector<PiecePtr> mPieces;
    
    PiecePtr mActivePiece;
};



#endif /*PLAYER_H_*/