
#include "onyx/sys/sys.h"
#include "onyx/ui/brightness_dialog.h"
#include "onyx/ui/ui_utils.h"

namespace ui
{

static const QString FANCY_STYLE = "                                \
    QSlider::groove:horizontal                                      \
    {                                                               \
        border: 1px solid #bbb;                                     \
        background: white;                                          \
        height: 20px;                                               \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::sub-page:horizontal                                    \
    {                                                               \
        background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,  \
                                    stop: 0 #66e, stop: 1 #bbf);    \
        background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,   \
                                    stop: 0 #bbf, stop: 1 #55f);    \
        border: 1px solid #777;                                     \
        height: 20px;                                               \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::add-page:horizontal                                    \
    {                                                               \
        background: #fff;                                           \
        border: 1px solid #777;                                     \
        height: 20px;                                               \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::handle:horizontal                                      \
    {                                                               \
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,         \
                                    stop:0 #eee, stop:1 #ccc);      \
        border: 1px solid #777;                                     \
        height: 25px;                                               \
        width: 13px;                                                \
        margin-top: -5px;                                           \
        margin-bottom: -5px;                                        \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::handle:horizontal:hover                                \
    {                                                               \
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,         \
                                    stop:0 #fff, stop:1 #ddd);      \
        border: 1px solid #444;                                     \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::sub-page:horizontal:disabled                           \
    {                                                               \
        background: #bbb;                                           \
        border-color: #999;                                         \
    }                                                               \
    QSlider::add-page:horizontal:disabled                           \
    {                                                               \
        background: #eee;                                           \
        border-color: #999;                                         \
    }                                                               \
    QSlider::handle:horizontal:disabled                             \
    {                                                               \
        background: #eee;                                           \
        border: 1px solid #aaa;                                     \
        border-radius: 4px;                                         \
    }";



BrightnessDialog::BrightnessDialog(QWidget *parent)
: QDialog(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
, layout_(this)
, title_(0)
, slider_(0)
{
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Text);
    setWindowOpacity(0.65);

    createLayout();
    updateText();
}

BrightnessDialog::~BrightnessDialog(void)
{
}

void BrightnessDialog::createLayout()
{
    setFixedWidth(ui::safeParentWidget(parentWidget())->width());

    layout_.setContentsMargins(2, 2, 2, 2);

    title_.useTitleBarStyle();
    title_.setAlignment(Qt::AlignCenter);
    layout_.addWidget(&title_);

    slider_.setRange(0, 255);
    slider_.setOrientation(Qt::Horizontal);
    slider_.setFixedHeight(40);
    slider_.setStyleSheet(FANCY_STYLE);
    slider_.setValue(sys::SysStatus::instance().brightness());
    layout_.addWidget(&slider_);

    connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

void BrightnessDialog::updateText()
{
    title_.setText(tr("Brightness"));
}

void BrightnessDialog::onValueChanged(int v)
{
    qDebug() << "Value changed " << v;
    sys::SysStatus::instance().setBrightness(v);
}

};  // namespace ui



