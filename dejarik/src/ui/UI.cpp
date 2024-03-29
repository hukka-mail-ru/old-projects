/*
 * This code was created by Jeff Molofee '99 
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 * 
 * or for port-specific comments, questions, bugreports etc. 
 * email to leggett@eecs.tulane.edu
 */
#include <math.h>

#include "UI.h"
#include "System.h"

#define INTERIM_ANGLES 4 // smoothness of the circles
#define MAX_FRAME_TIME 50 // in milliseconds


using namespace std;


void UI::startup()
{
    TRY_BEGINS;
       
    // create representation of the cells
    vector<CellPtr> cells;
    mGame->getBoard()->getCells(cells);
    for(unsigned i=0; i<cells.size(); i++)
    {
        createCell(cells[i]);
    }
        
    mVideo.startup();
    TRY_RETHROW;
}



void UI::createCell(const CellPtr cell)
{
    // CENTRAL CIRCLE
    if(cell->c == 0)
    {
        for(unsigned i = 0; i < INTERIM_ANGLES*3; i++)
        {
            float a = M_PI/6.0*i;
            cell->x.push_back(CIRCLE_CENTER_X + RADIUS_1 * cos(a));
            cell->y.push_back(CIRCLE_CENTER_Y + RADIUS_1 * sin(a));
        }
        
        cell->x_center = CIRCLE_CENTER_X;
        cell->y_center = CIRCLE_CENTER_Y;
    }
    // CIRCLE 1
    else if(cell->c == 1)
    {
        createCellSegment(cell, RADIUS_1, RADIUS_2);
    }
    // CIRCLE 2
    else if(cell->c == 2)
    {        
        createCellSegment(cell, RADIUS_2, RADIUS_3);
    }
}

// a helper for createCell
void UI::createCellSegment(const CellPtr cell, float radius1, float radius2)
{
    // angles
    const float a1 = M_PI/6.0 * cell->r;
    const float a2 = a1 + M_PI/6.0;
    const float interim = (a2 - a1)/INTERIM_ANGLES;        
    
    float a = a1;
    for(unsigned i = 0; i<=INTERIM_ANGLES; i++) // BBBBBB
    {
        cell->x.push_back(CIRCLE_CENTER_X + radius2 * cos(a));
        cell->y.push_back(CIRCLE_CENTER_Y + radius2 * sin(a));
        a += interim;
    }

    for(unsigned i = 0; i<=INTERIM_ANGLES; i++) // DDDDDD
    {            
        a -= interim;
        cell->x.push_back(CIRCLE_CENTER_X + radius1 * cos(a));
        cell->y.push_back(CIRCLE_CENTER_Y + radius1 * sin(a));
    }
    
    // define center
    cell->x_center = CIRCLE_CENTER_X + (radius2 + radius1)/2 * cos((a2 + a1)/2);
    cell->y_center = CIRCLE_CENTER_Y + (radius2 + radius1)/2 * sin((a2 + a1)/2);
    
    
}


bool UI::isCellClicked(int x, int y, CellPtr& cell)
{
    TRY_BEGINS;
    
    vector<CellPtr> cells;
    mGame->getBoard()->getCells(cells);
    
    for(unsigned k = 0; k < cells.size(); k++)
    {
        unsigned npol = cells[k]->x.size();
        vector<float>& xp = cells[k]->x;
        vector<float>& yp = cells[k]->y;

        // is (x,y) inside the polygon (xp, yp)
        bool res = false;
        for (unsigned i = 0, j = npol - 1; i < npol; j = i++) 
        {
          if ((((yp[i]<=y) && (y<yp[j])) || ((yp[j]<=y) && (y<yp[i]))) &&
            (x > (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
              res = !res;
        }
        
        if(res)
        {
            cell = cells[k];
            
            for (unsigned i = 0; i < cells[k]->x.size(); i++) 
            {
                cout << "cell - " << k << " : " <<  cells[k]->x[i] << " : " << cells[k]->y[i] << endl;
            }
            
            cout << "cell " << cell->c <<  "." << cell->r << endl; 
            
            return true;
        }
    }
    
    TRY_RETHROW;    
    
    return false;

}


void UI::drawCell(const CellPtr& cell, bool clicked) 
{
    TRY_BEGINS;
    
    RGBA_Color color = RGBA_Color(1,1,1,1);
    

    switch(cell->selected)
    {
        case SEL_CLICKED:         color = RGBA_Color(0,0,1,0.5);  if(!clicked) return; break;
        case SEL_POSSIBLE_MOVE:   color = RGBA_Color(0.2, 1, 0.2,0.5);  if(clicked) return; break;
        case SEL_POSSIBLE_TARGET: color = RGBA_Color(1,0,0,0.5);  if(clicked) return; break;
        case SEL_POSSIBLE_PUSH:   color = RGBA_Color(1,0,1,0.5);  if(clicked) return; break;
      //  case SEL_NONE:            color = RGBA_Color(1,0,1,1);break;
        case SEL_NONE:            return;
    }

    vector<GLshort> vertex;
    for(unsigned i = 0; i<cell->x.size(); i++)
    {
        vertex.push_back(cell->x[i]);
        vertex.push_back(cell->y[i]);
    }
    
    mVideo.drawSolidPolygon(&vertex[0], vertex.size()/2, color);
    
    if(cell->c == 0) 
       mVideo.drawLineLoop(&vertex[0], vertex.size()/2, RGBA_Color(0,0,0,1), 1);
    
       
    TRY_RETHROW;
}

void UI::getActiveFields(const float x, const float y, unsigned& first, unsigned& second)
{
    if(x >= 0 && y >= 0)
    {
        first = 2;
        second = (x > y) ? 4 : 1; 
    }
    else if (x < 0 && y >= 0)
    {
        first = 1;
        second = (-x > y) ? 3 : 2;
    }
    else if (x < 0 && y < 0)
    {
        first = 3;
        second = (-x > -y) ? 1 : 4;
    }
    else if (x >= 0 && y < 0)
    {
        first = 4;
        second = (x > -y) ? 2 : 3;
    }
}

void UI::drawPiece(const PiecePtr& piece)
{
    TRY_BEGINS;
    
    if(!piece)
    {
        return;
    }
    
    RGBA_Color color = (piece->player.get() == mGame->getActivePlayer()) ?
            RGBA_Color(1,0,1,1) : RGBA_Color(1,1,1,1);

    if(mMoving)
    {
        color = RGBA_Color(1,1,1,1);
    }
    

    mVideo.drawSprite("pieces", piece->name, color, XY_CENTER,
                      piece->x,
                      piece->y,
                      piece->angle); 
       
    TRY_RETHROW;
}


void UI::drawMenu()
{
    TRY_BEGINS;
    
    // TODO some subscriptions in the menu
    mVideo.drawSprite("menu1", 0, RGBA_Color(1,1,1,1), XY_RIGHT_TOP, 0, -FIELD_TEXTURE_WIDTH, 0);
    mVideo.drawSprite("menu2", 0, RGBA_Color(1,1,1,1), XY_LEFT_TOP, 0, -FIELD_TEXTURE_WIDTH, 0);

    TRY_RETHROW;
}

void UI::drawBoard()
{
    TRY_BEGINS;
    mVideo.drawSprite("board1", 0, RGBA_Color(1,1,1,1), XY_RIGHT_TOP, 0, FIELD_TEXTURE_WIDTH, 0);
    mVideo.drawSprite("board2", 0, RGBA_Color(1,1,1,1), XY_LEFT_TOP, 0, FIELD_TEXTURE_WIDTH, 0);
    mVideo.drawSprite("board3", 0, RGBA_Color(1,1,1,1), XY_RIGHT_TOP, 0, 0, 0);
    mVideo.drawSprite("board4", 0, RGBA_Color(1,1,1,1), XY_LEFT_TOP, 0, 0, 0);
     
    TRY_RETHROW;
}




void UI::drawField()
{
    TRY_BEGINS;

    // When moving, draw only two fields the Piece goes through and the central field.
    if(mActivePiece && mMoving)
    {
        unsigned first = 0;
        unsigned second = 0;
        getActiveFields(mActivePiece->x, mActivePiece->y, first, second);
    
        ostringstream field1;
        ostringstream field2;
        field1 << "field" << first;
        field2 << "field" << second;
        
        BindXY bind1;
        switch(first)
        {
            case 1: bind1 = XY_RIGHT_BOTTOM; break;
            case 2: bind1 = XY_LEFT_BOTTOM; break;
            case 3: bind1 = XY_RIGHT_TOP; break;
            case 4: bind1 = XY_LEFT_TOP; break;
        }
        
        BindXY bind2;
        switch(second)
        {
            case 1: bind2 = XY_RIGHT_BOTTOM; break;
            case 2: bind2 = XY_LEFT_BOTTOM; break;
            case 3: bind2 = XY_RIGHT_TOP; break;
            case 4: bind2 = XY_LEFT_TOP; break;
        }
        
        mVideo.drawSprite(field1.str(), 0, RGBA_Color(1,1,1,1), bind1, 0, 0, 0);
        mVideo.drawSprite(field2.str(), 0, RGBA_Color(1,1,1,1), bind2, 0, 0, 0);
        
        mVideo.drawSprite("field_center", 0, RGBA_Color(1,1,1,1), XY_CENTER, 0, 0, 0);
    }
    else
    {
        mVideo.drawSprite("field1", 0, RGBA_Color(1,1,1,1), XY_RIGHT_BOTTOM, FIELD_TEXTURE_WIDTH - WINDOW_WIDTH/2, 0, 0);
        mVideo.drawSprite("field2", 0, RGBA_Color(1,1,1,1), XY_LEFT_BOTTOM, 0, 0, 0);
        mVideo.drawSprite("field3", 0, RGBA_Color(1,1,1,1), XY_RIGHT_TOP, FIELD_TEXTURE_WIDTH - WINDOW_WIDTH/2, 0, 0);
        mVideo.drawSprite("field4", 0, RGBA_Color(1,1,1,1), XY_LEFT_TOP, 0, 0, 0);    
    }
    
    mVideo.enableBlend();
    
    // draw all but clicked cell  
    if(!mMoving)
    {
        vector<CellPtr> cells;
        mGame->getBoard()->getCells(cells);
         
        for(unsigned i = 0; i < cells.size(); i++)
        {
            drawCell(cells[i], false);
        }
        
        // draw clicked cell
        for(unsigned i = 0; i < cells.size(); i++)
        {
            drawCell(cells[i], true);
        }
    }

    // only one piece  
    drawPiece(mActivePiece);
        
    mVideo.disableBlend();
  
    TRY_RETHROW;    
}


void UI::memorizeField()
{
    drawBoard();
    
    // draw all but the active (clicked) Piece   
    mVideo.enableBlend();
       
    vector<PiecePtr> pieces = mGame->getBoard()->getPieces();
    for(unsigned i = 0; i < pieces.size(); i++)
    {        
        if(pieces[i] != mActivePiece)
        {
            drawPiece(pieces[i]);
        }
    }
    
    mVideo.disableBlend();
    
    // memorize 
    mVideo.copyBufferIntoTexture("field1", 0, WINDOW_HEIGHT-FIELD_TEXTURE_WIDTH);
    mVideo.copyBufferIntoTexture("field2", WINDOW_WIDTH/2,                     WINDOW_HEIGHT-FIELD_TEXTURE_WIDTH);
    mVideo.copyBufferIntoTexture("field3", 0, WINDOW_HEIGHT-FIELD_TEXTURE_WIDTH*2);
    mVideo.copyBufferIntoTexture("field4", WINDOW_WIDTH/2,                     WINDOW_HEIGHT-FIELD_TEXTURE_WIDTH*2);
    mVideo.copyBufferIntoTexture("field_center", WINDOW_WIDTH/2-CENTRAL_TEXTURE_WIDTH/2,  WINDOW_HEIGHT-FIELD_TEXTURE_WIDTH-CENTRAL_TEXTURE_WIDTH/2);

}


void UI::drawAll()
{
    memorizeField();
    drawField();
    drawMenu();       
}


void UI::onMouseClick(int x, int y)
{
    TRY_BEGINS;
    
    // the coordinates center is the circle center
    x -= WINDOW_WIDTH/2;
    y = FIELD_TEXTURE_WIDTH - y;
     
    CellPtr cell;
    if(isCellClicked(x, y, cell))
    {
        // show Menu item
    //    menuItemName = (cell->piece) ? cell->piece->name : "default";
        
        // process the click
        BattleResult res = mGame->onCellClick(cell);
       
        if(res == RES_MOVE)
        {
            mGame->getBoard()->getMoveSteps(cell, mMoveSteps);
            mMoveSteps.insert(mMoveSteps.begin(), cell->piece->cellBeforeMoving);
        }
        
        switch(res)
        {
            case RES_CLICK:  cout << "RES_CLICK" << endl; break;
            case RES_MOVE: cout << "RES_MOVE" << endl; break;
            case RES_KILL: cout << "RES_KILL" << endl;break;
            case RES_PUSH: cout << "RES_PUSH" << endl;break;
            case RES_COUNTER_KILL: cout << "RES_COUNTER_KILL" << endl;break;
            case RES_COUNTER_PUSH: cout << "RES_COUNTER_PUSH" << endl;break;
            default: break;
        }
        
        PlayerPtr vinner;
        if(mGame->checkVictory(vinner))
        {
            cout << "GAME OVER. Vinner: " << vinner->getName() << endl;
            exit(0);
        }
        
        // Shamanize a litte, because we need the valid 'mActivePiece' value
        // until the piece stops moving.
        static PiecePtr oldActivePiece;
        mActivePiece = mGame->getActivePlayer()->getActivePiece();        
        if(mActivePiece)
        {
            oldActivePiece = mActivePiece;
        }
        else
        {
            mActivePiece = oldActivePiece;
        }
        
    }   
 
    
    TRY_RETHROW;
}

void UI::handleEvents()
{
    TRY_BEGINS;
    static bool init = true;  

    for(;;)
    {        
        mMoving = mAnimation.updateAll(mMoveSteps);
        
        if(init)
        {
            drawAll();            
            EDR_SwapBuffers();
            
            init = false;
        }

        
        long time1 = EDR_GetTime(); // start the timer 

             

        static bool afterMove = false;
        if(mMoving)
        {
            long time2 = EDR_GetTime(); // start the timer 
            
            drawField();
            EDR_SwapBuffers();
            
            long time3 = EDR_GetTime(); // start the timer 
  
            afterMove = true;  
        }
        else if(!mMoving && afterMove) // drawAll one more time after moving
        {
            drawField();
            EDR_SwapBuffers();
            
            afterMove = false;
        }


        

        // handle the events in the queue 
        EDR_Event event;
        if(EDR_PollEvent(event))
        {
            if(event.type == EVENT_LBUTTONDOWN && !mMoving) 
            {
                onMouseClick(event.button.x, event.button.y);
                
                drawAll();                   
                EDR_SwapBuffers();
            }
            else if(event.type == EVENT_QUIT || event.type == EVENT_RBUTTONDOWN) 
            {
                break;
            }
            else if(event.type = EVENT_EXPOSE)
            {
                drawAll();                  
                EDR_SwapBuffers();
            }
        }
       
        // a delay before the next iteration
        
        long drawingTime = EDR_GetTime() - time1;
        if(mMoving && drawingTime < MAX_FRAME_TIME) // we shouldn't draw too fast
        {
        //    EDR_Millisleep(MAX_FRAME_TIME - drawingTime);
        }
        
        if(!mMoving) // we shouldn't call EDR_PollEvent too often.
        {
            EDR_Millisleep(MAX_FRAME_TIME);
        }
        
    }
    
    /* clean ourselves up and exit */
   // mVideo.stop();
    
    TRY_RETHROW;
}





