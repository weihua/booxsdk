#include "onyx/ui/status_bar_item_wifi_connection.h"
#include "onyx/data/network_types.h"
#include "onyx/sys/sys.h"
#include "onyx/ui/ui_utils.h"

namespace ui
{

const int SPACING = 2;
const int MAX = 6;

StatusBarItemWifiConnection::StatusBarItemWifiConnection(QWidget *parent)
    : StatusBarItem(CONNECTION, parent)
    , information_ ("WIFI")
    , strength_(-1)
    , total_(MAX)
    , text_width_(0)
{
    //TODO 
    setConnectionInfomation(-1, MAX, NO_SERVICE);
    queryWifiStatus();

    //in case of ...
    QTimer::singleShot(2000, this, SLOT(queryWifiStatus()));
}

void StatusBarItemWifiConnection::queryWifiStatus()
{
    if (sys::SysStatus::instance().isWpaSupplicantRunning())
    {
        sys::SysStatus::instance().queryWifiStatus();
    }
}

StatusBarItemWifiConnection::~StatusBarItemWifiConnection(void)
{
}

bool StatusBarItemWifiConnection::setConnectionInfomation(const int strength,
                                                      const int total,
                                                      const int type)
{
    bool dirty = false;

    QString text = networkType(type);
    if (information_ != text)
    {
        information_ = text;
        dirty = true;
    }

    QFont font("", defaultFontPointSize());
    QFontMetrics metric(font);
    text_width_ = metric.width("WIFI");
    setFixedWidth(30 + SPACING + text_width_);

    if (strength_ != strength || total_ != total)
    {
        strength_ = strength;
        total_ = total;
        dirty = true;
    }

    if (dirty) 
    {
        update();
    }
    return dirty;
}


void StatusBarItemWifiConnection::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw text at first.
    painter.setPen(QPen(Qt::white));
    QFont font("", defaultFontPointSize());
    painter.setFont(font);
    QRect rc = rect();
    rc.setWidth(text_width_);
    painter.drawText(rc, Qt::AlignVCenter|Qt::AlignRight, information_);

    // Paint image now.
    QFontMetrics metric(font);
    rc = rect();
    rc.adjust(text_width_ + SPACING, SPACING, 0, -SPACING);
    paintBars(painter, rc, QBrush(QColor(125, 125, 125)), MAX);
    paintBars(painter, rc, QBrush(Qt::white), strength_);

    if (strength_ < 0)
    {
        painter.setPen(QPen(QBrush(QColor(125, 125, 125)), 3));
        QPoint start(rc.topLeft());
        start.rx() += 3;
        start.ry() += rc.height() / 3;
        painter.drawLine(start, rc.bottomRight());
    }
}

void StatusBarItemWifiConnection::paintBars(QPainter & p,
                                        const QRect & rc,
                                        QBrush brush,
                                        int bars)
{
    int x = rc.left();
    int y = rc.top();
    int w = (rc.width() - (MAX - 1) * SPACING) / MAX;
    int h = rc.height() / MAX;
    for(int i = 0; i <= bars; ++i)
    {
        QRect r(x, y + (MAX -i) * h, w, h * i);
        p.fillRect(r, brush);
        x += w + SPACING;
    }
}

void StatusBarItemWifiConnection::mousePressEvent(QMouseEvent *me)
{
    me->accept();
}

void StatusBarItemWifiConnection::mouseReleaseEvent(QMouseEvent *me)
{
    me->accept();
    emit clicked();
}

QString StatusBarItemWifiConnection::networkType(int type)
{
    static const QString types[] =
    {
        "WIFI",
        "WIFI",
        "WIFI", 
        "WIFI",
        "WIFI",
        "WIFI",
        "WIFI",
        "WIFI",
        "WIFI",
        "WIFI"
    };
    static const int size = sizeof(types)/sizeof(types[0]);
    if (type >=0 && type < size)
    {
        return types[type];
    }
    return QString();
}


}
