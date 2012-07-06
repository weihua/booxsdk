#ifndef ONYX_LINE_EDIT_H_
#define ONYX_LINE_EDIT_H_

#include <QtGui/QtGui>

namespace ui
{

/// Line edit for eink device. Remove unnecessary updates.
class OnyxLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    OnyxLineEdit(QWidget *parent, const QString &name = "");
    OnyxLineEdit(const QString & text, QWidget *parent, const QString &name = "");
    ~OnyxLineEdit();

    QString getName();

protected:
    void focusInEvent(QFocusEvent *e);

Q_SIGNALS:
    void getFocus(OnyxLineEdit *object);
    void setCheckByMouse(OnyxLineEdit *object);
    void enterKeyPressed(OnyxLineEdit *object);
    void outOfRange(QKeyEvent *ke);

protected:
    void keyReleaseEvent(QKeyEvent *ke);
    void keyPressEvent(QKeyEvent * ke);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    QString name_;      ///< name for keyboard dialog title
    bool out_of_range_;
};

};

#endif  // ONYX_LINE_EDIT_H_
