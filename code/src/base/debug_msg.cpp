#include "onyx/base/debug_msg.h"

using namespace base;

QDebug DebugMsg::debug()
{
    return qDebug();
}
void DebugMsg::debug(const QString &msg)
{
    qDebug() << msg;
}

QDebug DebugMsg::warning()
{
    return qWarning();
}
void DebugMsg::warning(const QString &msg)
{
    qWarning() << msg;
}

class DebugMsgForFunc::Impl {
public:
    Impl(const QString &funcName, const QString &enterMsg, const QString &exitMsg)
        : func_name_(funcName), enter_msg_(enterMsg), exit_msg_(exitMsg)
    {
    }

    QString func_name_;
    QString enter_msg_;
    QString exit_msg_;
};

DebugMsgForFunc::DebugMsgForFunc(const QString &funcName, const QString &enterMsg, const QString &exitMsg)
{
    d = new Impl(funcName, enterMsg, exitMsg);

    if (enterMsg.isEmpty()) {
        this->debug() << ">>>>>> " << funcName;
    }
    else {
        this->debug() << ">>>>>> " << funcName << ": " << enterMsg;
    }
}
DebugMsgForFunc::~DebugMsgForFunc()
{
    if (d->exit_msg_.isEmpty()) {
        this->debug("<<<<<< " + d->func_name_);
    }
    else {
        this->debug("<<<<<< " + d->func_name_ + ": " + d->exit_msg_);
    }

    delete d;
}
