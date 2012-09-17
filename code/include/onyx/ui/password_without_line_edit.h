
#ifndef PASSWORD_WITHOUT_LINE_EDIT_H_
#define PASSWORD_WITHOUT_LINE_EDIT_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "onyx_dialog.h"
#include "catalog_view.h"
#include "onyx_keyboard.h"

namespace ui
{

// Dialog for getting input with soft keyboard support.
class PasswordWithoutLineEdit: public QWidget
{
    Q_OBJECT

public:
    PasswordWithoutLineEdit(QWidget *parent = 0);

    ~PasswordWithoutLineEdit();
    void createLayout();
    void setPlainTextVisible(bool value = false);
    void updateKeyBoardTitle(QString title);
    void selectedItem(bool value);

public:
    void popup(const QString &text, const int &y);

Q_SIGNALS:
        void sendKeyToTatget(QKeyEvent *);
        void showPlainText(bool);

protected Q_SLOTS:
    void onItemActivated(CatalogView *catalog, ContentView *item, int user_data);
    void closeKeyboard();

private:

    void createShowPlainText();
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyReleaseEvent(QKeyEvent *ke);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout plain_layout_;
    QHBoxLayout top_layout_;

    OnyxLineEdit *line_edit_;
    OnyxPushButton close_;
    OnyxLabel title_;

    OnyxKeyboard keyboard_;
    CatalogView show_plain_text_;
    ODatas show_plain_text_datas_;

    bool show_plain_;
};

}   // namespace ui

#endif /* PASSWORD_WITHOUT_LINE_EDIT_H_ */
