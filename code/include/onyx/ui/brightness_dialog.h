#ifndef ONYX_BRIGHTNESS_DIALOG_H_
#define ONYX_BRIGHTNESS_DIALOG_H_

#include "onyx_dialog.h"
#include "onyx/sys/sys.h"

namespace ui
{

/// Clock dialog.
class BrightnessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightnessDialog(QWidget *parent = 0);
    ~BrightnessDialog(void);

public Q_SLOTS:
    void onValueChanged(int v);

private:
    void createLayout();
    void updateText();

private:
    QVBoxLayout layout_;
    OnyxLabel title_;
    QSlider slider_;
};


};  // namespace ui


#endif  // ONYX_BRIGHTNESS_DIALOG_H_
