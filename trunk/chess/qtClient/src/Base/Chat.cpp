#include <Chat.h>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Client.h>
#include <UI.h>
#include <MainWindow.h>
#include <XML.h>


Chat::Chat(QWidget* parent, ChatType type):
    QDialog(parent), mChatType(type)
{
    // No title, no [X] button, no bottom
    setWindowFlags(Qt::Widget);

    QString chatNode = (mChatType == CT_COMMON_CHAT) ? XML_NODE_COMMON_CHAT : XML_NODE_TABLE_CHAT;

    QString style =  XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << XML_NODE_STYLE);
    setStyleSheet(style);

    mColorMe       = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << XML_NODE_FONT << XML_NODE_ME << XML_NODE_COLOR);
    mColorOpponent = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << XML_NODE_FONT << XML_NODE_OPPONENT << XML_NODE_COLOR);


    mHistory = new ChatHistory(this, mChatType);
    mHistory->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mHistory->setReadOnly(true);

    if(mChatType == CT_TABLE_CHAT)
    {
        QObject::connect(Client::instance(), SIGNAL(tableChatMessage(const QString&)), this, SLOT(onChatMessage(const QString&)));
    }
    else if(mChatType == CT_COMMON_CHAT)
    {
        QObject::connect(Client::instance(), SIGNAL(commonChatMessage(const QString&)), this, SLOT(onChatMessage(const QString&)));
    }



}

void Chat::updatePos(OrientationStatus orientation)
{
    qDebug() << "Chat::updatePos: " << orientation;

    QString chatNode = (mChatType == CT_COMMON_CHAT) ? XML_NODE_COMMON_CHAT : XML_NODE_TABLE_CHAT;

    QString orientNode = XML_NODE_LANDSCAPE; //(orientation == OrientationVertical) ? XML_NODE_PORTRAIT : XML_NODE_LANDSCAPE;

    int x      = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << orientNode << XML_NODE_X).toInt();
    int y      = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << orientNode << XML_NODE_Y).toInt();
    int width  = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << orientNode << XML_NODE_WIDTH).toInt();
    int height = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << chatNode << orientNode << XML_NODE_HEIGHT).toInt();

   // int textOffset   = XML::instance().readValue(XML_ITEMS_FILENAME, path << chatNode << XML_NODE_TEXT_OFFSET).toInt(); path.clear();
   // int borderWidth  = XML::instance().readValue(XML_ITEMS_FILENAME, path << chatNode << XML_NODE_BORDER << XML_NODE_WIDTH).toInt(); path.clear();

    move(x, y);
    setFixedSize(width, height);

    mHistory->setMinimumSize(width, height);
    qDebug() << "Chat::updatePos: end";
}



void Chat::ChatHistory::mouseReleaseEvent(QMouseEvent * event)
{
    MainWindow::instance()->showChatMessageDialog(mChatType);
}


void Chat::onChatMessage(const QString& originalMessage)
{
    QString message = originalMessage;
    // 'text' -> text
    if(message.startsWith('\'') && message.endsWith('\''))
    {
        int len = message.length() - 2; // 2  ' symbols
        message = message.mid(1, len);
    }

    QString htmlText = mHistory->toHtml();

    // TODO move colors into items.xml
    QString username = Client::instance()->username() + ":";
    if(message.left(username.length()) == username)
    {
         htmlText += "<font color=\"" + mColorMe + "\">" + message + "</font>";
    }
    else
    {
        htmlText += "<font color=\""+ mColorOpponent + "\">" + message + "</font>";
    }

    mHistory->setHtml(htmlText);
    mHistory->adjustSize();
    mHistory->verticalScrollBar()->setSliderPosition(mHistory->verticalScrollBar()->maximum());
}


Chat::~Chat()
{
}