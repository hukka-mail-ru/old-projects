#ifndef CHAT_H_
#define CHAT_H_

#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QTableWidget>
#include <QString>
#include <Defines.h>
#include <orientation.h>

enum ChatState
{
    CHAT_OPEN,
    CHAT_CLOSED
};

class Chat: public QDialog
{
Q_OBJECT
public:
    Chat(QWidget* parent, ChatType type);

    ~Chat();

    void updatePos(OrientationStatus orientation);
    ChatState getState() { return mState; }

    // derived from QWidged
    void show();
    bool close();

    void enable();
    void disable();

private:
    enum ChatSender
    {
        CS_ME,
        CS_OPPONENT,
        CS_SERVER
    };

    class History: public QTextEdit
    {
    public:
        History(QWidget* parent, ChatType type);
        void addMessage(const QString& message, ChatSender chatSender);
    protected:
        virtual void mouseReleaseEvent(QMouseEvent * event);
    private:
        ChatType mChatType;

        // colors
        QString mColorMe;
        QString mColorOpponent;
        QString mColorServer;
    };

    class Userlist: public QTableWidget
    {
    public:
        Userlist(QWidget* parent, ChatType type);
        void addUser(const QString& userName);
        void removeUser(const QString& userName);
        void removeAll();
    protected:
        virtual void mouseReleaseEvent(QMouseEvent * event);
    private:
        void updateTable();
        QList<QString> mNames;
        ChatType mChatType;
    };


    QGraphicsTextItem* mHeader;
    QGraphicsScene* mParentScene;

    History* mHistory;
    Userlist* mUserlist;

//    QScrollArea* mScrollArea;
    QGraphicsRectItem* mBorder;

    ChatType mChatType;
    ChatState mState;

private slots:

    void onChatMessage   (const QString& message);
    void onChatUserOnline(const QString& userName);
    void onChatUserJoined(const QString& userName);
    void onChatUserLeft  (const QString& userName);

};

#endif /* CHAT_H_ */