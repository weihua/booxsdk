#include "onyx/ui/keyboard_without_line_edit.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/screen/screen_proxy.h"

namespace ui
{

const QString LABEL_STYLE = "           \
QLabel                                  \
{                                       \
     padding: 0px;                      \
     background: black;                 \
     font: 24px ;                       \
     color: white;                      \
 }";

static const QString BUTTON_STYLE =    "\
QPushButton                             \
{                                       \
    background: transparent;            \
    font-size: 14px;                    \
    border-width: 1px;                  \
    border-color: transparent;          \
    border-style: solid;                \
    color: black;                       \
    padding: 0px;                       \
}";

KeyboardWithoutLineEdit::KeyboardWithoutLineEdit(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
    , big_layout_(this)
    , top_layout_(0)
    , keyboard_(this)
    , close_("" ,0)
    , title_(0)
{
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    createLayout();
    QRect rc = qApp->desktop()->screenGeometry();
    setFixedSize(rc.width(), 345);
    move(0, rc.height() - 345);
    onyx::screen::watcher().addWatcher(this);
}

KeyboardWithoutLineEdit::~KeyboardWithoutLineEdit()
{
}

void KeyboardWithoutLineEdit::createLayout()
{
    setBackgroundRole(QPalette::Button);
    setContentsMargins(0, 0, 0, 0);

    title_.setStyleSheet(LABEL_STYLE);
    title_.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    title_.setFixedHeight(36);

    close_.setStyleSheet(BUTTON_STYLE);
    QPixmap close_pixmap(":/images/close.png");
    close_.setIcon(QIcon(close_pixmap));
    close_.setIconSize(close_pixmap.size());
    close_.setFocusPolicy(Qt::NoFocus);

    top_layout_.addWidget(&title_, 500);
    top_layout_.addWidget(&close_, 72, Qt::AlignVCenter);
    top_layout_.setContentsMargins(10, 0, 10, 0);

    big_layout_.setContentsMargins(2, 2, 2, 2);
    big_layout_.setSpacing(0);
    big_layout_.setStretch(0, 0);

    big_layout_.addLayout(&top_layout_);
    big_layout_.addWidget(&keyboard_);

    connect(&close_, SIGNAL(clicked()), this, SLOT(closeKeyboard()));

    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
}

void KeyboardWithoutLineEdit::closeKeyboard()
{
    setVisible(false);
}

void KeyboardWithoutLineEdit::updateKeyBoardTitle(QString title)
{
    title_.setText(title);
    onyx::screen::watcher().enqueue(&title_, onyx::screen::ScreenProxy::GU);
}

void KeyboardWithoutLineEdit::popup(const QString &text, const int &y)
{
    QRect rc = qApp->desktop()->screenGeometry();
    setFixedSize(rc.width(), height());
    if(y < (rc.height() - height()))
    {
        move(rc.x(), rc.height() - height());
    }
    else
    {
        move(0, 0);
    }

    keyboard_.initFocus();

    if (isHidden())
    {
        show();
    }
    update();
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GU);
}

void KeyboardWithoutLineEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QBrush brush(QColor(Qt::black));
    painter.setBrush(brush);
    QRectF rect(2, 0, width()-2, 36);
    QPainterPath p_path;
    p_path.addRect(rect);
    painter.drawPath(p_path);
}

void KeyboardWithoutLineEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (Qt::Key_Up != key
            && Qt::Key_Down != key
            && Qt::Key_Left != key
            && Qt::Key_Right != key)
    {
        emit sendKeyToTatget(event);
    }
}

void KeyboardWithoutLineEdit::keyReleaseEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Escape)
    {
        setVisible(false);
    }
}

}   // namespace ui
