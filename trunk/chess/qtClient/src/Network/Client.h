#ifndef CLIENT_H_
#define CLIENT_H_


#include <QObject>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QTcpSocket>
#include <QThread>
#include <QList>
#include <QReadWriteLock>
#include <QLinkedList>
#include <deferror.h>
#include "Defines.h"
#include "Pixmaps.h"


enum GameStatus
{
    GAM_STOPPED,
    GAM_OPPONENT_JOINED,
    GAM_STARTED
};


char getCRC(const QByteArray& data);
QString commandToString(quint32 service, char command);
QString serviceToString(quint32 service);


#pragma pack(1)


struct Param
{
        quint32         id;
        quint32         value;
        quint32         compareOperator;
        quint32         logicalOperator;
};






/*====================================================================================================
  __  __    __  ___  _  _  ____
 / _)(  )  (  )(  _)( \( )(_  _)
( (_  )(__  )(  ) _) )  (   )(
 \__)(____)(__)(___)(_)\_) (__)
====================================================================================================*/
class Client: public QObject
{
Q_OBJECT
friend class TestClient;
friend class TestClientChess;

        Client(const QString& name = "");

public:
        static Client* instance()
        {
            static Client* client = new Client();
            return client;
        }

        void connectToHost(const QNetworkProxy& proxy, const QString& hostName, quint16 port);
        void disconnectFromHost();

        // communication with SRV Server
        void login(const QString& username, const QString& passwd);
        void registerUser(const QString& username, const QString& passwd);

        // communication with Table Manager
        void createGameTable     (LOGICID logicID, const QList<Param>& params);
        void deleteGameTable     (LOGICID logicID, TABLEID tableID);

        void getMyGameTable      (LOGICID logicID);
        void getRandomGameTable  (LOGICID logicID, const QList<Param>& params);
        void getGameTableParams  (LOGICID logicID, TABLEID tableID);
    //  void setGameTableParam   (LOGICID logicID, TABLEID tableID, const Param& param); // not implemented on server
    //  void getGameTableParams  (LOGICID logicID, TABLEID tableID, const QList<PARAMID>& ids);
    //  void setGameTableParams  (LOGICID logicID, TABLEID tableID, const QList<Param>& params);
        void findGameTables      (LOGICID logicID, quint32 maxCount, const QList<Param>& params);

        // communication with Chess Server
        // Master
        void agreeToStartGame    (TABLEID tableID);
        // Slave
        void joinGameTable       (TABLEID tableID);
        // Both
        void getField            (TABLEID tableID);
        void step                (TABLEID tableID, const Move& move);
        void getPlayerID         (const QString& playerName);
        void surrender           (TABLEID tableID);
        void getTime             (TABLEID tableID);

        // draw
        void offerDraw           (TABLEID tableID);
        void replyDraw           (TABLEID tableID, bool agree);

        // communication with Chat Server
        void joinCommonChat       (LOGICID logicID);
        void leaveCommonChat      (LOGICID logicID);
        void sendCommonChatMessage(LOGICID logicID, const QString& message);

        void joinTableChat       (LOGICID logicID, TABLEID tableID);
        void leaveTableChat      (LOGICID logicID, TABLEID tableID);
        void sendTableChatMessage(LOGICID logicID, TABLEID tableID, const QString& message);


        GameStatus   getGameStatus()   { return mGameStatus; }
        void         setGameStatus(GameStatus status)   { mGameStatus = status; }

signals:

        void connectedToHost();
        void disconnectedFromHost();
        void authorized();
        void registered();
        void joined(TABLEID id);

        void opponentJoined();
        void opponentAgreed();

        void gameStarted();
        void gameOver(const QString& message);
        void gameTableCreated(TABLEID id);
        void gameTableDeleted();

        void drawOffered();
        void drawRejected(const QString& message);

        void gotMyGameTable(TABLEID id);
        void gotGameTables(const QList<TABLEID>& ids);
        void gotField(const Field& field, bool myMove, bool iAmWhite);
        void gotGameTableParams(const QString& playerName, qint32 rating, qint32 time2step, qint32 time2game);

        void gotMoveTime(quint32);
        void gotGameTime(quint32);

        void invalidMove();

        void commonChatMessage(const QString& message);
        void tableChatMessage(const QString& message);

        void error(const QString& what);

private:

        struct MessageHeader
        {
                char            sign;
                qint32          size;
                char            version;
                quint32         service;
                char            cmd;
        };

        struct Message
        {
            char cmd;
            quint32 service;
            QByteArray buffer;
        };

        void sendCmd(char service, char command, const QByteArray& data);

        void processMessageSRV (const MessageHeader& header, const QByteArray& buffer);
        void processMessageCHS (const MessageHeader& header, const QByteArray& buffer);
        void processMessageTBM (const MessageHeader& header, const QByteArray& buffer);
        void processMessageREG (const MessageHeader& header, const QByteArray& buffer);
        void processMessageCHAT(const MessageHeader& header, const QByteArray& buffer);

        QString mName;

        QTcpSocket mSocket;

        GameStatus   mGameStatus;

        bool mClientAuthorized;

        QTimer* mTimer;

private slots:

        void onConnected();
        void onDisconnected();
        void onError();
        void onReadyRead();
        void onTimeout();
};



#endif /*CLIENT_H_*/