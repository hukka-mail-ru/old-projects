#ifndef UI_H_
#define UI_H_


#include "../common/Macros.h"
#include "Game.h"
#include "Animation.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

CLASSPTR(UI);


class UI
{
public:
    
    UI(const GamePtr& game): mGame(game), mQuit(false), animation(game)
    {}
    
    void startup();
    
    void handleEvents();
    
private:
    
    enum Color
    {
        CL_WHITE,
        CL_BLACK,
        CL_GREEN,
        CL_BLUE,
        CL_RED
    };
    
    bool drawAll();
    void drawBoard();
    void drawCell(const CellPtr& cell, bool clicked);
    void drawPiece(const PiecePtr& piece);
    void drawActivePlayer();
            
    /*
     * Defines if the mouse click is valid (true/false).
     * if true, returns the clicked cell
     */
    bool isCellClicked(float x, float y,  CellPtr& cell);
    
    void onMouseClick(const SDL_Event& event);
    
    GamePtr mGame; 
    bool mQuit;    
    std::vector<CellPtr> mMoveSteps; 
    
    Animation animation;

};


#endif /*UI_H_*/