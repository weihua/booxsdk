#include "onyx/ui/volume_control.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/sys/sys.h"
#include "math.h"

namespace ui
{

static const int TIMEOUT = 3000;
static QVector<int> volumes;

// VolumeControlDialog
VolumeControlDialog::VolumeControlDialog(QWidget *parent)
    : QDialog(parent, static_cast<Qt::WindowFlags>(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint))
    , layout_(this)
    , image_(":/images/volume_strength.png")
    , current_(0)
    , min_(0)
    , max_(1)
    , pressing_value_(-1)
    , label_(0)
{
    SysStatus & sys_status = SysStatus::instance();
    SystemConfig conf;
    min_ = conf.minVolume();
    max_ = conf.maxVolume();
    volumes = conf.volumes();
    conf.close();

    current_ = sys_status.volume() - min_;

    // connect the signals with sys_state_
    connect(&sys_status, SIGNAL(volumeChanged(int, bool)), this, SLOT(setVolume(int, bool)));

    createLayout();
    setModal(true);
    setFixedSize(300, 300);
    //setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    setFocusPolicy(Qt::NoFocus);

    timer_.setSingleShot(true);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

VolumeControlDialog::~VolumeControlDialog()
{
}

void VolumeControlDialog::createLayout()
{
    // hbox to layout line edit and buttons.
    layout_.setContentsMargins(4, 4, 4, 4);
    layout_.setSpacing(2);
    layout_.addWidget(&label_, 0, Qt::AlignHCenter|Qt::AlignTop);
    label_.setAlignment(Qt::AlignCenter);

    QPixmap pixmap=QPixmap::fromImage(image_);
    label_.setPixmap(pixmap);
}

void VolumeControlDialog::done(int r)
{
    sys::SysStatus::instance().enableIdle(true);
    stopTimer();
    QDialog::done(r);
}

void VolumeControlDialog::resetTimer()
{
    timer_.stop();
    timer_.start(TIMEOUT);
}

int VolumeControlDialog::ensureVisible()
{
    show();
    const QRect screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - this->rect().center() );
   
    resetTimer();
    sys::SysStatus::instance().enableIdle(false);
    onyx::screen::watcher().addWatcher(this);
    int ret = exec();
    onyx::screen::watcher().removeWatcher(this);
    return ret;
}

void VolumeControlDialog::moveEvent(QMoveEvent *e)
{
}

void VolumeControlDialog::resizeEvent(QResizeEvent *e)
{
    QPainterPath p;
    p.addRoundedRect(rect(), 10, 10);
    QRegion maskedRegion(p.toFillPolygon().toPolygon());
    setMask(maskedRegion);
}

void VolumeControlDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QBrush(QColor(190, 190, 190)));

    for (int i = 0; i < volumes.size(); ++i)
    {
        painter.fillRect(rectForVolume(i), current_ >= volumes[i] ? Qt::black : Qt::white);
    }
}

void VolumeControlDialog::mouseMoveEvent(QMouseEvent *me)
{
    me->accept();
}

void VolumeControlDialog::mousePressEvent(QMouseEvent *me)
{
    // Check position.
    resetTimer();
    me->accept();

    QRect rc1, rc2;
    rc1 = rectForVolume(0);
    int value = -1;
    for(int i = 1; i < volumes.size(); ++i)
    {
        rc2 = rectForVolume(i);
        if (rc1.left() < me->pos().x() &&
            me->pos().x() < rc2.left() &&
            me->pos().y() > rc2.top())
        {
            value = volumes[i - 1];
            break;
        }
        rc1 = rc2;
    }

    if (value < 0)
    {
        rc1 = rectForVolume(0);
        rc2 = rectForVolume(volumes.size() - 1);
        if (me->pos().x() < rc1.left())
        {
            value = min_;
        }
        else if (me->pos().x() >= rc2.left())
        {
            value = max_;
        }
    }

    sys::SysStatus::instance().setVolume(value);
#ifndef BUILD_FOR_ARM
    setVolume(value, false);
#endif

}

void VolumeControlDialog::mouseReleaseEvent(QMouseEvent *me)
{
    me->accept();
    if (!rect().contains(me->pos()))
    {
        reject();
    }
}

void VolumeControlDialog::onScreenUpdateRequest()
{
    onyx::screen::instance().updateWidget(this, onyx::screen::ScreenProxy::GU, false, onyx::screen::ScreenCommand::WAIT_COMMAND_FINISH);
}


bool VolumeControlDialog::event(QEvent *e)
{
    int ret = QDialog::event(e);
    return ret;
}

void VolumeControlDialog::keyPressEvent(QKeyEvent *ke)
{
    ke->ignore();
}

void VolumeControlDialog::keyReleaseEvent(QKeyEvent *ke)
{
    int key = ke->key();
    if (key == Qt::Key_Escape)
    {
        done(QDialog::Rejected);
        ke->accept();
        return;
    }
    ke->ignore();
}

void VolumeControlDialog::stopTimer()
{
    timer_.stop();
}

void VolumeControlDialog::setVolume(int volume, bool is_mute)
{
    if (current_ == volume)
    {
        return;
    }
    current_ = volume;
    repaint();
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::DW, onyx::screen::ScreenCommand::WAIT_NONE);
}

void VolumeControlDialog::onTimeout()
{
    stopTimer();
    accept();
}

QRect VolumeControlDialog::rectForVolume(int index)
{
    int left = 0, right = 0, bottom = 10;
    int spacing = 4 * layout_.spacing();
    layout_.getContentsMargins(&left, 0, &right, 0);

    int x = spacing + left;
    int delta = 8;
    int my_width = (width() - left - right) / volumes.size() - spacing;

    x += (my_width + spacing) * index;
    int h = 30 + index * delta;
    return QRect(x, height() - h - bottom, my_width, h);
}

}   // namespace ui
