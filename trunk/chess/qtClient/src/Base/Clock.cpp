/*
 * Clock.cpp
 *
 *  Created on: Jul 5, 2010
 *      Author: ssy
 */

#include "Clock.h"
#include <Defines.h>
#include <Client.h>
#include <UI.h>
#include <XML.h>
#include <MainWindow.h>
#include <QDebug>

const int DEFAULT_TIME = 600; // :)

Clock::Clock(QGraphicsScene* parentScene, const QString& header,
             const char* updateSignal, const QString& xmlNodeName):
    mSeconds(DEFAULT_TIME),
    mParentScene(parentScene),
    mText(NULL),
    mHeader(header),
    mXMLNodeName(xmlNodeName)
{
    QString family = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << XML_NODE_FONT << XML_NODE_FAMILY);
    int size =       XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << XML_NODE_FONT << XML_NODE_SIZE).toInt();
    mActiveColor =   XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << XML_NODE_FONT << XML_NODE_COLOR << XML_NODE_ACTIVE);
    mInactiveColor = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << XML_NODE_FONT << XML_NODE_COLOR << XML_NODE_INACTIVE);

    mText = mParentScene->addText("",QFont(family, size));

    QObject::connect(Client::instance(), updateSignal, this, SLOT(onGotTime(quint32)));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void Clock::start()
{
    mTimer->start(1000);
}


void Clock::getServerTime()
{
    Client::instance()->getTime(UI::instance()->getGameTable());
}


void Clock::onGotTime(quint32 seconds)
{
    mSeconds = seconds;
 //   qDebug() << "onGotTime " << seconds;
}

void Clock::updatePos(OrientationStatus orientation)
{
    QString orientNode = (orientation == OrientationHorizontal) ? XML_NODE_LANDSCAPE : XML_NODE_PORTRAIT;

    int x = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << orientNode << XML_NODE_X).toInt();
    int y = XML::instance().readValue(XML_ITEMS_FILENAME, QList<QString>() << XML_NODE_CLOCKS << mXMLNodeName << orientNode << XML_NODE_Y).toInt();

    mText->setPos(x, y);
}


void Clock::onTimeout()
{
    if(mSeconds > 0)
    {
        mSeconds--;
    }
    else if(UI::instance()->getGameState() != GS_WAIT_FOR_OPPONENT)
    {
        mSeconds = DEFAULT_TIME;
        UI::instance()->surrender();
    }

    mText->setPlainText(mHeader + Game::seconds2hrs(mSeconds));

    QString color = (UI::instance()->getGameState() == GS_WAIT_FOR_OPPONENT) ? mInactiveColor : mActiveColor;
    mText->setDefaultTextColor( QColor(color) );
}

