
#include "onyx/ui/password_without_line_edit.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/screen/screen_proxy.h"

namespace ui
{

PasswordWithoutLineEdit::PasswordWithoutLineEdit(QWidget *parent)

    : OnyxDialog(parent)
    , big_layout_(&content_widget_)
//    , keyboard_(this)
//    , top_layout_(0)
{
    createLayout();
    QRect rc = qApp->desktop()->screenGeometry();
    setFixedSize(rc.width(), 389);

    onyx::screen::watcher().addWatcher(this);
}

PasswordWithoutLineEdit::~PasswordWithoutLineEdit()
{
}

void PasswordWithoutLineEdit::createLayout()
{
    vbox_.setSpacing(0);
    content_widget_.setBackgroundRole(QPalette::Button);
    content_widget_.setContentsMargins(0, 0, 0, 0);
    createShowPlainText();

    top_layout_.setStretch(0, 0);
    top_layout_.addWidget(&show_plain_text_);

    big_layout_.setContentsMargins(2, 2, 2, 2);
    big_layout_.setSpacing(0);
    big_layout_.setStretch(0, 0);
    big_layout_.addLayout(&top_layout_);
    big_layout_.addWidget(&keyboard_);

    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);

    connect(&show_plain_text_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));

}

void PasswordWithoutLineEdit::setPlainTextVisible(bool value)
{
    show_plain_text_.setVisible(value);
}

void PasswordWithoutLineEdit::updateKeyBoardTitle(QString title)
{
    updateTitle(title);
}

void PasswordWithoutLineEdit::selectedItem(bool value)
{
    show_plain_text_.visibleSubItems().front()->data()->insert(TAG_CHECKED, value);
}

void PasswordWithoutLineEdit::onItemActivated(CatalogView *catalog,
                                   ContentView *item,
                                   int user_data)
{
    if (catalog == &show_plain_text_)
    {
        bool checked = false;
        ContentView * checkbox = show_plain_text_.visibleSubItems().front();
        if (checkbox->data()->contains(TAG_CHECKED))
        {
            checked = qVariantValue<bool>(checkbox->data()->value(TAG_CHECKED));
        }
        checkbox->data()->insert(TAG_CHECKED, !checked);
        checkbox->repaint();

        update();
        onyx::screen::instance().flush(0, onyx::screen::ScreenProxy::GU);
        QApplication::processEvents();

        emit showPlainText(!checked);
    }
}

void PasswordWithoutLineEdit::createShowPlainText()
{
    const int height = defaultItemHeight();
    show_plain_text_.setSubItemType(CheckBoxView::type());
    show_plain_text_.setPreferItemSize(QSize(height, height));

    ODataPtr dd(new OData);
    dd->insert(TAG_TITLE, tr("Show Plain Text"));
    dd->insert(TAG_CHECKED, false);
    show_plain_text_datas_.push_back(dd);

    show_plain_text_.setFixedGrid(1, 1);
    show_plain_text_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    show_plain_text_.setFixedHeight(defaultItemHeight()+2*SPACING);
    show_plain_text_.setData(show_plain_text_datas_);
    show_plain_text_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
}

int PasswordWithoutLineEdit::popup(const QString &text, const int &y)
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

void PasswordWithoutLineEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (Qt::Key_Up != key
            && Qt::Key_Down != key
            && Qt::Key_Left != key
            && Qt::Key_Right != key)
    {
        emit sendKeyToTatget(event);

        qDebug() << "11111111111;keyPressEvent";
    }
}

}   // namespace ui

