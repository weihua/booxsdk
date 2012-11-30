
#include "onyx/ui/password_without_line_edit.h"
#include "onyx/ui/onyx_keyboard_utils.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/screen/screen_proxy.h"
#include "onyx/sys/platform.h"

namespace ui
{

static const QString LABEL_STYLE = "    \
QLabel                                  \
{                                       \
     padding: 0px;                      \
     background: transparent;           \
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
PasswordWithoutLineEdit::PasswordWithoutLineEdit(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint)
    , big_layout_(this)
    , close_("", 0)
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
    setBackgroundRole(QPalette::Button);
    setContentsMargins(0, 0, 0, 0);

    title_.setStyleSheet(LABEL_STYLE);
    title_.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    title_.setFixedHeight(36);

    close_.setStyleSheet(BUTTON_STYLE);
    QPixmap close_pixmap(":/images/close.png");
    close_.setIconSize(close_pixmap.size());
    close_.setIcon(QIcon(close_pixmap));
    close_.setFocusPolicy(Qt::NoFocus);
    if(sys::isNoTouch())
    {
        close_.setVisible(false);
    }

    top_layout_.addWidget(&title_, 500);
    top_layout_.addWidget(&close_, 72, Qt::AlignVCenter);
    top_layout_.setContentsMargins(10, 0, 10, 0);

    createShowPlainText();

    plain_layout_.setStretch(0, 0);
    plain_layout_.addWidget(&show_plain_text_);

    big_layout_.setContentsMargins(2, 2, 2, 2);
    big_layout_.setSpacing(0);
    big_layout_.setStretch(0, 0);
    big_layout_.addLayout(&top_layout_);
    big_layout_.addLayout(&plain_layout_);
    big_layout_.addWidget(&keyboard_);

    keyboard_.menu()->setNeighbor(&show_plain_text_, CatalogView::RECYCLE_UP);
    show_plain_text_.setNeighbor(keyboard_.menu(), CatalogView::RECYCLE_DOWN);

    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GC);

    connect(&show_plain_text_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
            this, SLOT(onItemActivated(CatalogView *, ContentView *, int)));
    connect(&close_, SIGNAL(clicked()), this, SLOT(closeKeyboard()));
}

void PasswordWithoutLineEdit::setPlainTextVisible(bool value)
{
    show_plain_text_.setVisible(value);
}

void PasswordWithoutLineEdit::closeKeyboard()
{
    setVisible(false);
}

void PasswordWithoutLineEdit::updateKeyBoardTitle(QString title)
{
    title_.setText(title);
    onyx::screen::watcher().enqueue(&title_, onyx::screen::ScreenProxy::GU);
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
    const int height = 36;
    show_plain_text_.setSubItemType(CheckBoxView::type());
    show_plain_text_.setPreferItemSize(QSize(height, height));

    ODataPtr dd(new OData);
    dd->insert(TAG_TITLE, tr("Show Plain Text"));
    dd->insert(TAG_CHECKED, false);
    show_plain_text_datas_.push_back(dd);

    show_plain_text_.setFixedGrid(1, 1);
    show_plain_text_.setMargin(OnyxKeyboard::CATALOG_MARGIN);
    show_plain_text_.setFixedHeight(height+4);
    show_plain_text_.setData(show_plain_text_datas_);
    show_plain_text_.setNeighbor(keyboard_.top(), CatalogView::DOWN);
}

void PasswordWithoutLineEdit::popup(const QString &text, const int &y)
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

void PasswordWithoutLineEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QBrush brush(Qt::black);
    painter.setBrush(brush);
    QRectF rect(2, 0, width()-2, 38);
    QPainterPath p_path;
    p_path.addRect(rect);
    painter.drawPath(p_path);
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
    }
}

void PasswordWithoutLineEdit::keyReleaseEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Escape)
    {
        setVisible(false);
    }
}

}   // namespace ui

