#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <Defines.h>
#include <Button.h>
#include <Cell.h>
#include <Chat.h>
#include <orientation.h>
#include <MoveBox.h>
#include <Board.h>



class GameScene: public QGraphicsScene
{
    Q_OBJECT
public:


    GameScene(QObject *parent = 0);

    ~GameScene();

    void initialize();
    void update();

    void enableItems();
    void disableItems();

    void showChat();

    void updateItemsPositions(OrientationStatus orientation);

    void close();


private:

    void updateMoveBoxes(GameState gameState);

    Button* mMenuButton;
    Button* mExitButton;

    Chat* mChat;

    MoveBox mMeMoveBox;
    MoveBox mOppMoveBox;

    Board mBoard;

private slots:

    void onGotField(const Field& field, bool myMove, bool iAmWhite);
    void onMenuButtonClicked();
    void onExitClicked();

};


#endif /* SCENE_H_ */