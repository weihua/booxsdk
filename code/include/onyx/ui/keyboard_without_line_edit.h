
#ifndef KEYBOARD_WITHOUT_LINE_EDIT_H_
#define KEYBOARD_WITHOUT_LINE_EDIT_H_

#include "onyx/base/base.h"
#include "ui_global.h"
#include "onyx_dialog.h"
#include "catalog_view.h"
#include "onyx_keyboard.h"

namespace ui
{

// Dialog for getting input with soft keyboard support.
class KeyboardWithoutLineEdit: public QWidget
{
    Q_OBJECT

public:
    KeyboardWithoutLineEdit(QWidget *parent = 0);
    ~KeyboardWithoutLineEdit();

    void updateKeyBoardTitle(QString title);

public:
    void popup(const QString &text, const int &y);

Q_SIGNALS:
    void sendKeyToTatget(QKeyEvent *);
    void requitUpdate();

private Q_SLOTS:
    void closeKeyboard();

protected:
    void paintEvent(QPaintEvent *e);

private:
    void createLayout();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QVBoxLayout big_layout_;
    QHBoxLayout top_layout_;

    OnyxLineEdit *line_edit_;
    OnyxKeyboard keyboard_;

    OnyxPushButton close_;
    OnyxLabel title_;

    int x_;
    int y_;
};

}   // namespace ui


#endif /* KEYBOARD_WITHOUT_LINE_EDIT_H_ */
