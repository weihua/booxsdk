#include <algorithm>
#include "onyx/ui/menu_item.h"
#include "onyx/sys/platform.h"
#include "onyx/sys/sys_status.h"

namespace ui
{

static const int SPACING = 3;

static const QBrush SELECTED_BRUSH(QColor(0, 0, 0, 255));
static const QBrush NORMAL_BRUSH(QColor(128, 128, 128, 128));

MenuItem::MenuItem(QWidget *parent, QAction *action)
    : QWidget(parent)
    , action_(0)
    , is_layout_dirty_(true)
    , require_background_(true)
{
    // setAutoFillBackground(true);
    // setBackgroundRole(QPalette::Base);
    setAction(action);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MenuItem::~MenuItem()
{
}

/// Can return a hardcode value for all menu items.
QSize MenuItem::sizeHint() const
{
    if(ui::isHD() && sys::isIRTouch())
    {
        return QSize(105, 113);
    }
    return QSize(85, 95);
}

QSize MenuItem::minimumSizeHint() const
{
    if(ui::isHD() && sys::isIRTouch())
    {
        return QSize(105, 113);
    }
    return QSize(85, 95);
}

bool MenuItem::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
        e->accept();
        return true;

    default:
        break;
    }
    return QWidget::event(e);
}

/// TODO, change it to better rendering.
void MenuItem::paintEvent(QPaintEvent *e)
{
    if (!updateLayout())
      {
          return;
      }

      QPainter p(this);

      static const int OFFSET = 1;
      static const int ROUNDED = 3;
      QPainterPath path;
      QRect rc = rect().adjusted(OFFSET*2, OFFSET*2, -OFFSET*2, -OFFSET*2);

      QRect paint_rc;
      paint_rc = rect().adjusted(OFFSET, OFFSET, -OFFSET, -OFFSET);
      path.addRoundedRect(paint_rc, ROUNDED, ROUNDED, Qt::AbsoluteSize);
      rc.adjust(OFFSET, OFFSET, -OFFSET, -OFFSET);

      QPen pen(Qt::SolidLine);
      if (action_->isEnabled())
      {
          pen.setColor(Qt::black);
      }
      else
      {
          pen.setColor(Qt::lightGray);
      }

      if (hasFocus())
      {
          pen.setColor(Qt::black);
          pen.setBrush(Qt::black);
          pen.setWidth(2);
          p.setPen(pen);
          p.drawPath(path);
      }

      if(require_background_)
      {
          pen.setWidth(2);
          pen.setColor(QColor(76, 76, 76));
          p.setPen(pen);
          p.setBrush(Qt::gray);
          p.drawRoundRect(rc);
      }

      QRect icon_rect(icon_pos_, iconActualSize());
      if (action_->isChecked())
      {
          action_->icon().paint(&p, icon_rect,
                                Qt::AlignTop|Qt::AlignHCenter,
                                QIcon::Selected, QIcon::On);
      }
      else if (action_->isEnabled())
      {
          action_->icon().paint(&p, icon_rect,
                                Qt::AlignTop|Qt::AlignHCenter);
      }
      else if (!action_->isEnabled())
      {
          action_->icon().paint(&p, icon_rect,
                                Qt::AlignTop|Qt::AlignHCenter,
                                QIcon::Disabled);
      }

      // Draw text.
      pen.setColor(Qt::black);
      p.setPen(pen);
      title_layout_.draw(&p, QPoint(0, 0));
}

void MenuItem::mousePressEvent(QMouseEvent *e)
{
    e->accept();
}

void MenuItem::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    click();
}

void MenuItem::click()
{
    if (action_ == 0 || !action_->isEnabled())
    {
        return;
    }

    setFocus();
    action_->trigger();
    emit clicked(this, action_);
}

void MenuItem::setAction(QAction *action)
{
    if (action == 0)
    {
        setFocusPolicy(Qt::NoFocus);
    }
    else
    {
        setFocusPolicy(Qt::TabFocus);
    }

    if (action_ == action)
    {
        return;
    }

    action_ = action;
    if (action_)
    {
        connect(action_, SIGNAL(triggered(bool)), this, SLOT(onActionChanged(bool)));
        connect(action_, SIGNAL(toggled(bool)), this, SLOT(onActionChanged(bool)));
    }
    is_layout_dirty_ = true;
    update();
}

void MenuItem::onActionChanged(bool)
{
    update();
}

bool MenuItem::updateLayout()
{
    if (action_ == 0)
    {
        return false;
    }

    int w = 0;
    qreal h = 0.0;

    if (is_layout_dirty_)
    {
        title_layout_.setText(action_->text());
        title_layout_.setFont(action_->font());
        title_layout_.setCacheEnabled(false);
        QTextOption option = title_layout_.textOption();
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        title_layout_.setTextOption(option);
        title_layout_.beginLayout();

        QTextLine line = title_layout_.createLine();
        while (line.isValid())
        {
            line.setLineWidth(rect().width()-8);
            layout_width_ = static_cast<int>(line.naturalTextRect().width());
            if (w < layout_width_)
            {
                w = layout_width_;
            }

            line.setPosition(QPointF((rect().width() - layout_width_) >> 1, h));
            h += line.height()-1;
            line = title_layout_.createLine();
        }
        title_layout_.endLayout();
    }

    title_layout_.setPosition(QPoint(0, iconActualSize().height() + 2));

    // icon.
    icon_pos_.rx() = ((rect().width() - iconActualSize().width()) >> 1);
    icon_pos_.ry() = SPACING;

    is_layout_dirty_ = false;

    return true;
}

QSize MenuItem::iconActualSize() const
{
    return action_->icon().actualSize(QSize(1024, 1024));
}

void MenuItem::setRequireBackground(bool require)
{
    require_background_ = require;
}

bool MenuItem::getRequereBackground()
{
    return require_background_;
}



Section::Section()
    : layout_(0)
    , selected_row_(0)
    , selected_column_(0)
    , current_focus_(0)
{
}

Section::~Section()
{
}

int Section::navigate(int direction)
{
    int index = focusItem();
    if (index < 0)
    {
        return index;
    }

    Qt::Key k = static_cast<Qt::Key>(direction);
    switch (k)
    {
    case Qt::Key_Up:
        index = prevRow(index);
        break;
    case Qt::Key_Down:
        index = nextRow(index);
        break;
    case Qt::Key_Left:
    case Qt::Key_PageUp:
        index = prevColumn(index);
        break;
    case Qt::Key_Right:
    case Qt::Key_PageDown:
        index = nextColumn(index);
        break;
    default:
        return -1;
    }

    if (index >=0 && index < static_cast<int>(items_.size()))
    {
        items_.at(index)->setFocus();
        return index;
    }
    return -1;
}

bool Section::activate()
{
    int index = focusItem();
    if (index >= 0)
    {
        items_.at(index)->click();
        current_focus_ = index;
        return true;
    }
    return false;
}

void Section::clear()
{
    MenuItemsIter begin, end, iter;
    begin = items_.begin();
    end   = items_.end();
    for(iter = begin; iter != end; ++iter)
    {
        delete *iter;
    }
    items_.clear();
}

bool Section::setFocus(int row, int col)
{
    int index = row * layout().columnCount() + col;
    if (index >= 0 && index < static_cast<int>(items_.size()))
    {
        items_.at(index)->setFocus();
        return true;
    }
    return false;
}

bool Section::hasFocus()
{
    return (focusItem() >= 0);
}

int Section::currentFocusItem()
{
    return current_focus_;
}

void Section::setCurrentFocusItem(int index)
{
    current_focus_ = index;
}

MenuItem *Section::createItem(QWidget *parent, QAction *action)
{
    MenuItem * item = new MenuItem(parent, action);
    connect(item, SIGNAL(clicked(MenuItem*, QAction *)), this, SLOT(onClicked(MenuItem*, QAction *)));
    return item;
}

/// Search the one which has focus.
int Section::focusItem()
{
    MenuItemsIter begin, end, iter;
    begin = items_.begin();
    end   = items_.end();
    for(iter = begin; iter != end; ++iter)
    {
        if ((*iter)->hasFocus())
        {
            return (iter - begin);
        }
    }
    return -1;
}

int Section::prevRow(int pos)
{
    // Could not move.
    if (layout().rowCount() <= 1)
    {
        return -1;
    }

    int new_pos = pos - layout().columnCount();
    for(int i = new_pos; i >= 0; --i)
    {
        if (i < static_cast<int>(items_.size()) && items_.at(i)->action())
        {
            return i;
        }
    }

    return -1;
}

int Section::prevColumn(int pos)
{
    // Could not move.
    if (layout().columnCount() <= 1)
    {
        return -1;
    }

    while (pos >= 0)
    {
        int col = pos % layout().columnCount();

        if (col <= 0)
        {
            return -1;
        }

        --pos;
        if (pos >= 0 && pos < static_cast<int>(items_.size()) && items_.at(pos)->action())
        {
            return pos;
        }
    }

    return -1;
}

int Section::nextRow(int pos)
{
    // Could not move.
    if (layout().rowCount() <= 1)
    {
        return -1;
    }

    int new_pos = pos + layout().columnCount();
    for(int i = new_pos; i > pos; --i)
    {
        if (i >= 0 && i < static_cast<int>(items_.size()) && items_.at(i)->action())
        {
            return i;
        }
    }

    return -1;
}

int Section::nextColumn(int pos)
{
    // Could not move.
    if (layout().columnCount() <= 1)
    {
        return -1;
    }

    while (++pos > 0 && pos < static_cast<int>(items_.size()))
    {
        if (items_.at(pos)->action())
        {
            return pos;
        }
    }

    return -1;
}

/// Add a new item to section.
MenuItem *Section::addItem(QWidget *parent, QAction *action, int row, int col)
{
    MenuItem * item = createItem(parent, action);
    items_.push_back(item);
    layout_.setSpacing(0);
    layout_.setVerticalSpacing(0);
    layout_.setHorizontalSpacing(0);
    layout_.addWidget(item, row, col);

    if (action && action->isChecked())
    {
        current_focus_ = row;
    }

    return item;
}

bool Section::arrangeItems(QWidget *parent, BaseActions *base_actions, int rows, int columns)
{
    if (base_actions == 0)
    {
        return false;
    }

    vector<shared_ptr<QAction> > & actions = base_actions->actions();
    int size = actions.size();
    int max = rows * columns;
    if (size > max)
    {
        qWarning("Too many items.");
        qDebug() << "size: " << size << "max: " <<max;
    }

    // Create menu items for the group.
    int bound = columns * rows;
    if (static_cast<int>(items_.size()) < bound)
    {
        for(int r = 0; r < rows; ++r)
        {
            for(int c = 0; c < columns; ++c)
            {
                MenuItem *wnd = createItem(parent, 0);
                items_.push_back(wnd);
                layout_.setSpacing(0);
                layout_.setVerticalSpacing(0);
                layout_.setHorizontalSpacing(0);
                layout_.addWidget(wnd, r, c);
            }
        }
    }

    // Consider the separator.
    bool focus = false;
    QAction * act = 0;
    int action_index = 0;
    int i = 0;
    while (i < max)
    {
        act = 0;
        if (action_index < size)
        {
            act = actions[action_index].get();
        }

        if (act && !act->isSeparator())
        {
            items_[i]->setAction(act);
            if (act->isChecked())
            {
                items_[i]->setFocus();
                selected_row_ = i / columns;
                selected_column_ = i % columns;
                focus = true;
            }
            ++i;
        }
        else if (act)
        {
            if(!isLandscapeMode())
            {
                int begin = (i / columns) * columns;
                int end = begin + columns;
                for(; i != begin && i < end;++i)
                {
                    items_[i]->setAction(0);
                }
            }
        }
        else
        {
            items_[i++]->setAction(0);
        }
        ++action_index;
    }

    if (!focus)
    {
        selected_row_ = 0;
        selected_column_ = 0;
        items_.at(0)->setFocus();
        focus = true;
    }
    return focus;
}

void Section::onClicked(MenuItem *wnd, QAction *action)
{
    int i = focusItem();
    if (i >= 0)
    {
        current_focus_ = i;
    }
    emit clicked(wnd, action);
}

}
