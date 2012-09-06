#ifndef DEBUG_MSG_H
#define DEBUG_MSG_H

#include <QDebug>

namespace base
{

class DebugMsg
{
public:
    QDebug debug()
    {
        return qDebug();
    }
    void debug(const QString &msg)
    {
        qDebug() << msg;
    }

    QDebug warning()
    {
        return qWarning();
    }
    void warning(const QString &msg)
    {
        qWarning() << msg;
    }
};

class DebugMsgForFunc : public DebugMsg
{
public:
    DebugMsgForFunc(const QString &funcName, const QString &enterMsg, const QString &exitMsg)
        : func_name_(funcName), enter_msg_(enterMsg), exit_msg_(exitMsg)
    {
        if (enterMsg.isEmpty()) {
            this->debug(">>>>>> " + funcName);
        }
        else {
            this->debug(">>>>>> " + funcName + ": " + enterMsg);
        }
    }
    ~DebugMsgForFunc()
    {
        if (exit_msg_.isEmpty()) {
            this->debug("<<<<<< " + func_name_);
        }
        else {
            this->debug("<<<<<< " + func_name_ + ": " + exit_msg_);
        }
    }

private:
    QString func_name_;
    QString enter_msg_;
    QString exit_msg_;
};

}

#endif // DEBUG_MSG_H
