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

    void enableItems();
    void disableItems();

    void showChat();
    void startClocks();

    void updateItemsPositions(OrientationStatus orientation);

    void close();


private:

    void updateGameField(const Field& field, bool white);
    void updateMoveBoxes(GameState gameState);

    QObject *mParent;


    QGraphicsItem* mNote;

    Button* mMenuButton;
    Button* mExitButton;


    void loadImages();

    Chat* mChat;

    MoveBox mMeMoveBox;
    MoveBox mOppMoveBox;

    bool mWhite;

    Board mBoard;

private slots:

    void onGotField(const Field& field, bool myMove, bool iAmWhite);
    void onMenuButtonClicked();
    void onExitClicked();

};


#endif /* SCENE_H_ */
