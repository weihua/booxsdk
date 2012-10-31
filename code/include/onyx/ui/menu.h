/// Popup menu.
#ifndef UI_POPUP_MENU_H_
#define UI_POPUP_MENU_H_

#include <map>
#include <QtGui/QtGui>
#include "menu_item.h"


#include "encoding_actions.h"
#include "font_family_actions.h"
#include "font_actions.h"
#include "zoom_setting_actions.h"
#include "edit_actions.h"
#include "bookmark_actions.h"
#include "reading_tools_actions.h"
#include "system_actions.h"
#include "view_actions.h"
#include "onyx/ui/sketch_actions.h"
#include "onyx/ui/label.h"
#include "onyx/ui/buttons.h"

namespace ui
{

class PopupMenu : public QDialog
{
    Q_OBJECT

public:
    PopupMenu(QWidget *parent, bool load_translator = false);
    ~PopupMenu();

public:
    void addGroup(BaseActions * actions);
    void setSystemAction(BaseActions * actions);
    int  popup(const QString &menu_title = "");
    QAction * selectedCategory();
    // must be called after all categories have been added
    void setSelectedCategory(QAction *category);
    void clear();

protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual bool event(QEvent * event);
    virtual void resizeEvent(QResizeEvent *);
    virtual void showEvent(QShowEvent * event);

private:
    void createMenuLayout();
    void addCategory(QAction *group);
    bool arrangeItems(BaseActions *items);
    int  rows();
    int  columns();

    void navigate(int key);
    void activate();

    void updateClickedItem(QWidget *wnd);
    bool loadTranslator();

    void resizeRoundRectDialog();
    bool isLandscapeMode();

public Q_SLOTS:
    virtual void done(int);

private Q_SLOTS:
    void onItemClicked(MenuItem* wnd, QAction *action);
    void onGroupClicked(MenuItem* wnd, QAction *action);
    void onSystemActionClicked(MenuItem* wnd, QAction *action);
    void onClosePressed();
    void onCloseClicked();

private:
    QAction * selected_category_;
    QActionGroup exclusive_group_;  ///< To make category exclusive.

    std::map<QAction *, BaseActions *> all_actions_;
    BaseActions* system_actions_;

    QVBoxLayout big_layout_;

    QWidget menu_title_widget_;
    QWidget system_title_widget_;

    QHBoxLayout menu_layout_;

    Section categroy_section_;
    Section children_section_;
    Section system_section_;

    QHBoxLayout system_title_layout_;
    QHBoxLayout menu_title_layout_;

    OnyxLabel system_title_label_;
    OnyxLabel menu_title_label_;
    OnyxLabel system_icon_label_;
    OnyxLabel menu_icon_label_;
    OnyxPushButton close_button_;
};

}

#endif // UI_POPUP_MENU_H_
