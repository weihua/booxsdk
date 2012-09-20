#ifndef DEBUG_MSG_H
#define DEBUG_MSG_H

#include <QDebug>
#include <QTime>

namespace base
{

class DebugMsg
{

public:
    QDebug debug();
    QDebug warning();
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

class ProfileUtil
{
private:
    ProfileUtil();

public:
    static void log(const QString &msg);

    /// prety printing methods to profile a task's start and end time
    static void start(const QString &msg);
    static void end(const QString &msg);

private:
    class Impl;
};

}

#endif // DEBUG_MSG_H
