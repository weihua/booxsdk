#include "onyx/ui/keyboard_without_line_edit.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/screen/screen_proxy.h"

namespace ui
{

KeyboardWithoutLineEdit::KeyboardWithoutLineEdit(QWidget *parent)

    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
    , keyboard_(this)
{
    createLayout();
    QRect rc = qApp->desktop()->screenGeometry();
    setFixedSize(rc.width(), 349);

    onyx::screen::watcher().addWatcher(this);
}

KeyboardWithoutLineEdit::~KeyboardWithoutLineEdit()
{
}

void KeyboardWithoutLineEdit::createLayout()
{
    vbox_.setSpacing(0);
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);

    big_layout_.setContentsMargins(2, 2, 2, 2);
    big_layout_.setSpacing(0);
    big_layout_.setStretch(0, 0);
    big_layout_.addWidget(&keyboard_);

    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);
}

void KeyboardWithoutLineEdit::updateKeyBoardTitle(QString title)
{
    updateTitle(title);
}

int KeyboardWithoutLineEdit::popup(const QString &text, const int &y)
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

    int ret = exec();
    return ret;
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

}   // namespace ui
