

#include "onyx/touch/touch_listener.h"


TouchEventListener::TouchEventListener()
: socket_(this)
{
    QObject::connect(&socket_, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

TouchEventListener::~TouchEventListener()
{
}

bool TouchEventListener::connect()
{
    socket_.connectToServer(TOUCH_SERVER_ADDRESS);
    return true;
}

bool TouchEventListener::disconnect()
{
    socket_.disconnectFromServer();
    return true;
}

void TouchEventListener::onReadyRead()
{
    static int count = 0;
    int bytes = socket_.bytesAvailable();
    int time = bytes / sizeof(TouchData);
    for(int i = 0; i < time; ++i)
    {
        TouchData d;
        socket_.read(reinterpret_cast<char *>(&d), sizeof(d));
        emit touchData(d);
     }
}

