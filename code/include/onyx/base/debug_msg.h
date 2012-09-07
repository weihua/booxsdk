#ifndef DEBUG_MSG_H
#define DEBUG_MSG_H

#include <QDebug>

namespace base
{

class DebugMsg
{
public:
    QDebug debug();
    void debug(const QString &msg);

    QDebug warning();
    void warning(const QString &msg);
};

class DebugMsgForFunc : public DebugMsg
{
public:
    DebugMsgForFunc(const QString &funcName, const QString &enterMsg, const QString &exitMsg);
    ~DebugMsgForFunc();

private:
    class Impl;
    Impl *d;
};

}

#endif // DEBUG_MSG_H
