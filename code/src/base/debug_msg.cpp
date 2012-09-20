#include "onyx/base/debug_msg.h"

using namespace base;

QDebug DebugMsg::debug()
{
    return qDebug();
}

QDebug DebugMsg::warning()
{
    return qWarning();
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
        this->debug() << "<<<<<< " << d->func_name_;
    }
    else {
        this->debug() << "<<<<<< " << d->func_name_ << ": " << d->exit_msg_;
    }

    delete d;
}

class ProfileUtil::Impl {
public:
    static qint64 last_timestamp_;
};

qint64 ProfileUtil::Impl::last_timestamp_ = 0;

void ProfileUtil::log(const QString &msg)
{
    QDateTime now =  QDateTime::currentDateTime();
    qint64 t = now.toMSecsSinceEpoch();
    qDebug() << QString("TIMING: %1, %2, (%3ms)").arg(msg).arg(now.toString()).arg(t - ProfileUtil::Impl::last_timestamp_);
    ProfileUtil::Impl::last_timestamp_ = t;
}

void ProfileUtil::start(const QString &msg)
{
    log(">>>>>> " + msg);
}

void ProfileUtil::end(const QString &msg)
{
    log("<<<<<< " + msg);
}
