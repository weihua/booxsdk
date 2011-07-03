#ifndef ONYX_TOUCH_EVENT_LISTENER_H_
#define ONYX_TOUCH_EVENT_LISTENER_H_

#include <QObject>
#include <QLocalSocket>
#include "touch_data.h"


class TouchEventListener : public QObject
{
    Q_OBJECT

public:
    TouchEventListener();
    ~TouchEventListener();

public:
    bool connect();
    bool disconnect();

public Q_SLOTS:
    void onReadyRead();

Q_SIGNALS:
    void touchData(TouchData & data);

private:
    QLocalSocket socket_;
};


#endif
