#include "onyx/base/device.h"
#include "onyx/ui/status_bar_item_app.h"

namespace ui
{

StatusBarItemApp::StatusBarItemApp(QWidget *parent)
    : StatusBarItem(APP_DEFINED, parent)
    , image_(":/images/app_default.png")
{
    id_ = -1;
    createLayout();
}

StatusBarItemApp::~StatusBarItemApp(void)
{
}

void StatusBarItemApp::setAppId(const int id)
{
    id_ = id;
}

int StatusBarItemApp::appId()
{
    return id_;
}

void StatusBarItemApp::setImage(const QImage & image)
{
    image_ = image;
    update();
}

void StatusBarItemApp::createLayout()
{
    setFixedWidth(image_.width());
}

void StatusBarItemApp::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    QPoint point;
    point.rx() = ((rect().width() - image_.width()) >> 1);
    point.ry() = ((rect().height() - image_.height()) >> 1);
    painter.drawImage(point, image_);
}

void StatusBarItemApp::mousePressEvent(QMouseEvent *me)
{
    me->accept();
}

void StatusBarItemApp::mouseReleaseEvent(QMouseEvent *me)
{
    me->accept();
    emit clicked(appId());
}

}
