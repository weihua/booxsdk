#include "onyx/ui/status_bar_item_viewport.h"

namespace ui
{

StatusBarItemViewport::StatusBarItemViewport(QWidget *parent)
    : StatusBarItem(VIEWPORT, parent)
{
    setFixedWidth(30);
}

StatusBarItemViewport::~StatusBarItemViewport(void)
{
}

void StatusBarItemViewport::setViewport(const QRect & parent,
                                        const QRect & child)
{
    parent_ = parent;
    child_ = child;
    if (child_.width() > parent_.width() || child_.height() > parent_.height())
    {
        QRect tmp = parent_;
        parent_ = child_;
        child_ = tmp;
    }

    // scale and switch.
    const int WIDTH = width() - 2;
    double scale = parent_.width() / WIDTH;
    QRect bk(0, 0, WIDTH, static_cast<int>(parent_.height() / scale));
    QRect fg(child_.left() / scale, child_.top() / scale, child_.width() / scale, child_.height() / scale);

    parent_ = bk;
    child_ = fg;
    update();
}

void StatusBarItemViewport::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.fillRect(parent_.adjusted(1, 1, -1, -1), QBrush(QColor(100, 100, 100)));
    painter.setPen(Qt::white);
    painter.drawRect(parent_);
    painter.fillRect(child_.adjusted(1, 1, -1, -1), QBrush(QColor(255, 255, 255)));
}

}
