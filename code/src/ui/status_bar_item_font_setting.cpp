#include "onyx/ui/status_bar_item_font_setting.h"

namespace ui
{

StatusBarItemFontSetting::StatusBarItemFontSetting(QWidget *parent)
    : StatusBarItem(FONT_SETTING, parent)
{
    createLayout();
}

StatusBarItemFontSetting::~StatusBarItemFontSetting(void)
{
}

void StatusBarItemFontSetting::createLayout()
{
    QImage & img = image();
    setFixedWidth(img.width());
}

void StatusBarItemFontSetting::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    QImage & img = image();
    QPoint point;
    point.rx() = ((rect().width() - img.width()) >> 1);
    point.ry() = ((rect().height() - img.height()) >> 1);
    painter.drawImage(point, img);
}

void StatusBarItemFontSetting::mousePressEvent(QMouseEvent *me)
{
    me->accept();
}

void StatusBarItemFontSetting::mouseReleaseEvent(QMouseEvent *me)
{
    me->accept();
    emit clicked();
}

QImage & StatusBarItemFontSetting::image()
{
    if (images_.isEmpty())
    {
        images_.insert(0, QImage(resourcePath()));
    }
    return images_[0];
}

QString StatusBarItemFontSetting::resourcePath()
{
    QString str(":/images/font_settings_small.png");
    str = getIconPath(str);
    return str;
}
}
