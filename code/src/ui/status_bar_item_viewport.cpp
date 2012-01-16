#include "onyx/ui/status_bar_item_viewport.h"

namespace ui
{

StatusBarItemViewport::StatusBarItemViewport(QWidget *parent)
    : StatusBarItem(VIEWPORT, parent)
{
    setFixedWidth(28);
}

StatusBarItemViewport::~StatusBarItemViewport(void)
{
}

void StatusBarItemViewport::setViewport(const QRect & parent,
                                        const QRect & child)
{
    parent_ = parent;
    child_ = child;
    update();
}

void StatusBarItemViewport::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.fillRect(parent_, QBrush(QColor(0, 0, 0)));
    painter.setPen(Qt::white);
    painter.drawRect(parent_);
    painter.fillRect(child_, QBrush(QColor(255, 255, 255)));
}

}
