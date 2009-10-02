#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>



class Exception
{
public:

    Exception(const QString msg = QString::null): mMsg(msg) { qDebug() << "EXCEPTION THROWN:" << mMsg; }
    
    Exception(const char* file, int line, const QString msg = QString::null): mMsg(msg) 
    { 
        QString str("EXCEPTION! (");
        str += file;
        str += ": ";
        str += line;
        str += ") ";
        str += mMsg;
        qDebug() << str; 
    }

    QString what() const { return mMsg; }

private:

    QString mMsg;

};

#endif

