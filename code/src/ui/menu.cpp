
#include <algorithm>
#include "onyx/ui/ui_global.h"
#include "onyx/ui/menu.h"
#include "onyx/ui/menu_item.h"
#include "onyx/ui/keyboard_navigator.h"
#include "onyx/screen/screen_proxy.h"
#include "onyx/screen/screen_update_watcher.h"
#include "onyx/sys/sys_status.h"

namespace ui
{

using namespace std;

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
}                                       \
QPushButton:pressed                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: black;                \
    background-color: black;            \
}                                       \
QPushButton:checked                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: black;                \
    background-color: black;            \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";

const QString TITLE_STYLE = "           \
QLabel                                  \
{                                       \
     padding: 0px;                      \
     background: transparent;           \
     font: 24px ;                       \
     color: white;                      \
 }";

const static int MARGIN = 2;
const static int RND = 25;
const static int PEN_WIDTH = 2;
const static int OUT_WIDTH = 4;

static QTime popup_time;

const int MIN_ELAPSED = 800;
const int STATE_HIDE = 0;
const int STATE_INITIAL_VISIBLE = 1;
const int STATE_IGNORE = 2;
static int state = STATE_HIDE;

PopupMenu::PopupMenu(QWidget *parent, bool load_translator)
: QDialog(parent, Qt::Popup|Qt::FramelessWindowHint)
, selected_category_(0)
, exclusive_group_(this)
, system_actions_(0)
, big_layout_(this)
, menu_title_widget_(0)
, system_title_widget_(0)
, menu_layout_(0)
, categroy_section_()
, children_section_()
, system_section_()
, system_title_layout_(0)
, menu_title_layout_(0)
, system_title_label_(tr("Menu"), &system_title_widget_)
, menu_title_label_(tr("Option"), 0)
, system_icon_label_("", &system_title_widget_)
, menu_icon_label_("", 0)
{
    createMenuLayout();
    onyx::screen::watcher().flush(this, onyx::screen::ScreenProxy::GU);
}

PopupMenu::~PopupMenu()
{
    clear();
}

// Create menu.
void PopupMenu::createMenuLayout()
{
    QString background_sytle("background-color: black");

    big_layout_.setContentsMargins(0, 0, 0, 0);

    menu_title_layout_.setContentsMargins(8, 0, 80, 0);
    menu_title_layout_.addWidget(&menu_icon_label_, 0, Qt::AlignLeft);
    menu_title_layout_.addWidget(&menu_title_label_, 0, Qt::AlignLeft);
    menu_title_layout_.addStretch();

    menu_icon_label_.setPixmap(QPixmap(":/images/option_icon.png"));

    menu_title_label_.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    menu_title_label_.setStyleSheet(TITLE_STYLE);

    big_layout_.addLayout(&menu_title_layout_);
    big_layout_.addLayout(&menu_layout_);

    if(isLandscapeMode())
    {
        // Bottom category section.

        menu_layout_.setSpacing(0);
        categroy_section_.layout().setContentsMargins(2, 0, 0, 0);
        big_layout_.addLayout(&categroy_section_.layout());

        // Childrent section.
        children_section_.layout().setContentsMargins(0, 0, 2, 0);
        menu_layout_.addLayout(&children_section_.layout());
    }
    else
    {
        // Left category section.

        menu_layout_.setSpacing(0);
        categroy_section_.layout().setContentsMargins(2, 0, 0, 0);
        menu_layout_.addLayout(&categroy_section_.layout());

        // Childrent section.
        children_section_.layout().setContentsMargins(0, 0, 2, 0);
        menu_layout_.addLayout(&children_section_.layout());

    }


    big_layout_.addWidget(&system_title_widget_);

    system_title_widget_.setStyleSheet(background_sytle);
    system_title_widget_.setLayout(&system_title_layout_);

    system_title_layout_.setContentsMargins(8, 0, 4, 0);
    system_title_layout_.addWidget(&system_icon_label_, 0, Qt::AlignLeft);
    system_title_layout_.addWidget(&system_title_label_, 0, Qt::AlignLeft);
    system_title_layout_.addStretch();

    system_icon_label_.setPixmap(QPixmap(":/images/menu_icon.png"));

    system_title_label_.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    system_title_label_.setStyleSheet(TITLE_STYLE);

    system_section_.layout().setContentsMargins(2, 0, 2, 0);
    big_layout_.addLayout(&system_section_.layout());

    resizeRoundRectDialog();
    // Setup connection.
    connect(&categroy_section_, SIGNAL(clicked(MenuItem *, QAction *)),
            this, SLOT(onGroupClicked(MenuItem *, QAction *)));
    connect(&children_section_, SIGNAL(clicked(MenuItem *, QAction *)),
            this, SLOT(onItemClicked(MenuItem *, QAction *)));
    connect(&system_section_, SIGNAL(clicked(MenuItem *, QAction *)),
            this, SLOT(onSystemActionClicked(MenuItem *, QAction *)));
}

void PopupMenu::resizeRoundRectDialog(void)
{
    if(!ui::isHD())
    {
        QPixmap pixmap(":/images/menu_background.png");
        if(isLandscapeMode())
        {
            pixmap = QPixmap(":/images/menu_background_landscape.png");
        }
        QPainterPath path;
        int dialog_width = pixmap.width();
        int dialog_height = pixmap.height();
        QRectF rect = QRectF(0, 0, dialog_width, dialog_height);
        path.addRoundRect(rect, 3, 3);
        QPolygon polygon = path.toFillPolygon().toPolygon();
        QRegion region(polygon);
        setMask(region);
    }
}

void PopupMenu::addCategory(QAction *category)
{
    MenuItem * item = 0;
    if(isLandscapeMode())
    {
        item = categroy_section_.addItem(this, category, 1, categroy_section_.items().size());
    }
    else
    {
        item = categroy_section_.addItem(this, category, categroy_section_.items().size(), 0);
    }

    item->setRequireBackground(false);
    if (category->isChecked())
    {
        selected_category_ = category;
    }
}

/// Arrange items belong to the group. Before using this function, caller
/// should make sure the group has been correctly initialized.
/// \param items The items belong to the group.
/// \return This function returns true when one item in the group is selected.
/// Otherwise it returns false.
bool PopupMenu::arrangeItems(BaseActions *items)
{
    // Arrange all children items for this group.
    return children_section_.arrangeItems(this, items, rows(), columns());
}

int PopupMenu::rows()
{
    if(isLandscapeMode())
    {
        return 3;
    }
    return 5;

}

int  PopupMenu::columns()
{
    if(isLandscapeMode())
    {
        return 7;
    }
    return 5;
}

void PopupMenu::keyPressEvent(QKeyEvent *ke)
{
    ke->accept();
}

void PopupMenu::keyReleaseEvent(QKeyEvent *ke)
{
    // Check the current selected type.
    ke->accept();

    if (popup_time.elapsed() < MIN_ELAPSED)
    {
        return;
    }

    switch (ke->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
    case Qt::Key_PageUp:
        navigate(ke->key());
        break;
    case Qt::Key_Return:
        activate();
        break;
    case ui::Device_Menu_Key:
    case Qt::Key_Escape:
    case Qt::Key_Home:
        reject();
        break;
    }
}

/// Navigate according to the key.
void PopupMenu::navigate(int key)
{
    QWidget * wnd = ui::moveFocus(this, key);
    if (wnd)
    {
        onyx::screen::instance().enableUpdate(false);
        wnd->setFocus();
        repaint();
        onyx::screen::instance().enableUpdate(true);
        onyx::screen::instance().flush(this, onyx::screen::ScreenProxy::DW, false);
    }
}

void PopupMenu::activate()
{
    if (categroy_section_.hasFocus())
    {
        categroy_section_.activate();
    }
    else if (children_section_.hasFocus())
    {
        children_section_.activate();
    }
    else if (system_section_.hasFocus())
    {
        system_section_.activate();
    }
}

void PopupMenu::paintEvent(QPaintEvent *pe)
{
    QPainter p(this);

    int index = categroy_section_.currentFocusItem();
    MenuItem * item = categroy_section_.items()[index];
    QPoint item_point = item->mapToParent(QPoint(0, 0));

    int w = item->rect().width() + OUT_WIDTH;
    int h = item->rect().height();

    int rw = float(w) * float(RND) / 100.0;
    int rh = float(h) * float(RND) / 100.0;

    int x = MARGIN;
    int y = item_point.y() - MARGIN;

    if(!isLandscapeMode())
    {
        QString image_path(":/images/menu_background.png");
        p.drawPixmap(rect(), QPixmap(image_path));

        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::white, PEN_WIDTH, Qt::SolidLine));
        p.drawRoundRect(x, y, w, h, RND, RND);
        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::black, PEN_WIDTH, Qt::SolidLine));
        int start_angle_up = 90* 16;
        int span_angle_up = 90 * 16;
        p.drawArc(x, y, rw, rh, start_angle_up, span_angle_up);
        int start_angle_down = 180 * 16;
        int span_angle_down = 90 * 16;
        p.drawArc(x, y + h - rh, rw, rh, start_angle_down, span_angle_down);

        p.drawLine(x + rw / 2, y, x + w - MARGIN * 5, y);
        p.drawLine(x + rw / 2, y + h, x + w - MARGIN * 5, y + h);
        p.drawLine(x, y + rh / 2, x, y + h - rh / 2);
    }
    else
    {
        p.drawPixmap(rect(), QPixmap(":/images/menu_background_landscape.png"));

        x = item_point.x() - MARGIN;
        y = item_point.y() - MARGIN;

        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::white, PEN_WIDTH, Qt::SolidLine));
        p.drawRect(x, y, w, h/2);
        p.drawRoundRect(x, y + h/2, w, h/2);

        p.setPen(QPen(Qt::black, PEN_WIDTH, Qt::SolidLine));
        int start_angle_left = 180 * 16;
        int span_angle_left = 90 * 16;

        p.drawArc(x, y + h - rh, rw, rh, start_angle_left, span_angle_left);
        int start_angle_right = 270 * 16;
        int span_angle_right = 90 * 16;
        p.drawArc(x + w - rw, y + h - rh, rw, rh, start_angle_right, span_angle_right);

        p.drawLine(x + rw / 2, y + h, x + w - MARGIN * 5, y + h);
        p.drawLine(x, y + MARGIN * 3, x, y + h - rh / 2);
        p.drawLine(x + w, y + MARGIN * 3 , x + w, y + h - rh / 2);
    }
    if(!sys::isNoTouch())
    {
        p.drawPixmap(rect().x()+rect().width()-60, rect().y()+2, QPixmap(":/images/close.png"));
    }
    onyx::screen::watcher().enqueue(0, onyx::screen::ScreenProxy::GU);
}

void PopupMenu::resizeEvent(QResizeEvent *)
{

}

void PopupMenu::done(int r)
{
    onyx::screen::watcher().enqueue(0, onyx::screen::ScreenProxy::GU);
    return QDialog::done(r);
}

void PopupMenu::showEvent(QShowEvent * event)
{
    QDialog::showEvent(event);
    state = STATE_INITIAL_VISIBLE;
}

void PopupMenu::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
}

void PopupMenu::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->pos().x()>(this->width()-60) && event->y() < 60)
    {
        qDebug() << "mouse release reject!";
        reject();
    }
    QDialog::mouseReleaseEvent(event);
}

QAction * PopupMenu::selectedCategory()
{
    if (selected_category_ == 0 && categroy_section_.items().size() > 0)
    {
        return categroy_section_.items().front()->action();
    }
    return selected_category_;
}

void PopupMenu::setSelectedCategory(QAction *category)
{
    int size = categroy_section_.items().size();
    for (int i=0; i<size; i++)
    {
        if (categroy_section_.items().at(i)->action() == category)
        {
            selected_category_ = category;
            categroy_section_.setCurrentFocusItem(i);
        }
    }
}
void PopupMenu::updateClickedItem(QWidget *wnd)
{
    onyx::screen::instance().enableUpdate(false);
    repaint();
    onyx::screen::instance().enableUpdate(true);
    onyx::screen::instance().updateWidget(
        this,
        onyx::screen::ScreenProxy::DW,
        false,
        onyx::screen::ScreenCommand::WAIT_ALL);
}

void PopupMenu::onItemClicked(MenuItem* wnd, QAction *action)
{
    // Make it selected and update the user interface.
    updateClickedItem(wnd);
    accept();
}

void PopupMenu::onGroupClicked(MenuItem* wnd, QAction *action)
{
    onyx::screen::instance().enableUpdate(true);
    selected_category_ = action;
    arrangeItems(all_actions_[action]);
    update();
    onyx::screen::instance().flush(this,onyx::screen::ScreenProxy::GU);
}

/// We are able to handle some system action directly in
/// popup menu.
void PopupMenu::onSystemActionClicked(MenuItem* wnd, QAction *action)
{
    // Check which one has been selected.
    selected_category_ = system_actions_->category();
    updateClickedItem(wnd);
    accept();
}

/// Add a group to the popup menu.
void PopupMenu::addGroup(BaseActions * actions)
{
    all_actions_[actions->category()] = actions;
    actions->category()->setActionGroup(&exclusive_group_);
    addCategory(actions->category());
}

/// Set the system action. There is only one system action.
void PopupMenu::setSystemAction(BaseActions * actions)
{
    system_actions_ = actions;
    system_section_.arrangeItems(this, actions, 1, 6);
}

int PopupMenu::popup(const QString &)
{
    // Reset visible.
    state = STATE_HIDE;

    QAction *group = selectedCategory();
    QGridLayout & layout = categroy_section_.layout();
    layout.setVerticalSpacing(5);
    layout.setRowStretch(layout.rowCount(), 20);

    // Disable the screen update now. Most applications will
    // update the screen when popup menu is shown. The reason is
    // that Qt system asks these applications to paint themselves
    // when menu is shown. The Qt framebuffer is updated but it's not necessary
    // to update the screen here.
    onyx::screen::watcher().addWatcher(this);

    arrangeItems(all_actions_[group]);
    setModal(true);
    // show();

    // Record current time to avoid the flick.
    // grabMouse();
    popup_time.start();

    // Ensure all pending events has been processed.
    // Use pre-defined timer to prevent deadlock.
    QTime t;
    t.start();
    while (QCoreApplication::hasPendingEvents() && t.elapsed() < 1500)
    {
        QCoreApplication::processEvents();
    }
    QCoreApplication::removePostedEvents(0);

    QRect rect = ui::safeParentWidget(parentWidget())->rect();

    QPixmap pixmap;
    if(!isLandscapeMode())
    {
        pixmap = QPixmap(":/images/menu_background.png");
    }
    else
    {
        pixmap = QPixmap(":/images/menu_background_landscape.png");
    }
    if(!ui::isHD())
    {
        if(!isLandscapeMode())
        {
            move((rect.width()-pixmap.width()), (rect.height()-pixmap.height())/2);
        }
        else
        {
            move((rect.width()-pixmap.width()) / 2, (pixmap.height()-rect.height()));
        }
    }

    onyx::screen::watcher().enqueue(0, onyx::screen::ScreenProxy::GC);
    int ret = exec();

    // releaseMouse();
    onyx::screen::instance().flush(3000);

    if (ret == QDialog::Rejected)
    {
        // Post process.
        onyx::screen::instance().flush(
            0,
            onyx::screen::ScreenProxy::GC,
            true,
            onyx::screen::ScreenCommand::WAIT_ALL);
    }
    return ret;
}

void PopupMenu::clear()
{
    categroy_section_.clear();
    children_section_.clear();
    system_section_.clear();
}

bool PopupMenu::loadTranslator()
{
    // Not sure it will cause memory leak.
    static QTranslator *pre_translator;
    QTranslator *translator = new QTranslator;
    QString file = "onyx_ui_" + QLocale::system().name() + ".qm";

    // Double check.
    QDir dir(QDir::home());
    QString path = SHARE_ROOT;
    if (!path.isEmpty())
    {
        dir.cd(path);
    }

    if (!dir.cd("translations"))
    {
        return false;
    }

    qDebug("Locale file %s", qPrintable(dir.absoluteFilePath(file)));
    translator->load(dir.absoluteFilePath(file));

    if (pre_translator)
    {
        QApplication::removeTranslator(pre_translator);
    }
    QApplication::installTranslator(translator);
    pre_translator = translator;
    return true;
}

bool PopupMenu::event(QEvent * e)
{
    bool ret = QDialog::event(e);
    if (e->type() == QEvent::UpdateRequest)
    {
        if (state == STATE_INITIAL_VISIBLE)
        {
            onyx::screen::instance().flush(
                this,
                onyx::screen::ScreenProxy::GC,
                false,
                onyx::screen::ScreenCommand::WAIT_ALL);
            state = STATE_IGNORE;
            onyx::screen::instance().enableUpdate(false);
        }
        else if (state == STATE_IGNORE)
        {
            onyx::screen::instance().updateWidget(this, onyx::screen::ScreenProxy::GU);
        }
    }
    return ret;
}

}
