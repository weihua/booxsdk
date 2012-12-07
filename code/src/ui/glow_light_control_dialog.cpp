
#include "onyx/sys/sys.h"
#include "onyx/ui/ui_utils.h"
#include "onyx/ui/glow_light_control_dialog.h"
#include "onyx/data/data_tags.h"
#include "onyx/screen/screen_update_watcher.h"

namespace ui
{

static const int ITEM_HEIGHT = 50;
static const int MY_WIDTH = 20;
static const int MY_HEIGHT = 240;

static const QString FANCY_STYLE = "                                \
    QSlider::groove:horizontal                                      \
    {                                                               \
        border: 2px solid black;                                    \
        background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,  \
                                    stop: 0 #6666e, stop: 1 #b66bf);\
        height: 40px;                                               \
        border-radius: 4px;                                         \
    }                                                               \
    QSlider::sub-page:horizontal                                    \
    {                                                               \
        background: black;                                          \
        border: 2px solid black;                                    \
        height: 40px;                                               \
        border-radius: 0px;                                         \
    }                                                               \
    QSlider::add-page:horizontal                                    \
    {                                                               \
        background: white;                                          \
        border: 2px solid black;                                    \
        height: 40px;                                               \
        border-radius: 0px;                                         \
    }                                                               \
    QSlider::handle:horizontal                                      \
    {                                                               \
        background: white;                                          \
        border: 2px solid black;                                    \
        height: 45px;                                               \
        width: 30px;                                                \
        margin-top: -5px;                                           \
        margin-bottom: -5px;                                        \
        border-radius: 0px;                                         \
    }                                                               \
    QSlider::handle:horizontal:hover                                \
    {                                                               \
        background: white;                                          \
        border: 2px solid black;                                    \
        border-radius: 0px;                                         \
    }                                                               \
    QSlider::sub-page:horizontal:disabled                           \
    {                                                               \
        background: black;                                          \
        border-color: #999;                                         \
    }                                                               \
    QSlider::add-page:horizontal:disabled                           \
    {                                                               \
        background: black;                                          \
        border-color: white;                                        \
    }                                                               \
    QSlider::handle:horizontal:disabled                             \
    {                                                               \
        background: black;                                          \
        border: 2px solid black;                                    \
        border-radius: 4px;                                         \
    }";



GlowLightControlDialog::GlowLightControlDialog(QWidget *parent)
: QDialog(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint)
, layout_(this)
, slider_h_layout_(0)
, h_layout_(0)
, ok_h_layout_(0)
, title_(0)
, slider_(0)
, switch_view_(0, this)
, ok_view_(0, this)
{
    setModal(true);

//    setAutoFillBackground(true);
//    setBackgroundRole(QPalette::Base);
//    setWindowOpacity(0.8);

    createLayout();
    updateText();

    onyx::screen::watcher().addWatcher(this);
}

GlowLightControlDialog::~GlowLightControlDialog(void)
{
}

int GlowLightControlDialog::exec()
{
    int margin = 20;
    int offset = 40;
    int width = QApplication::desktop()->screenGeometry().width() - 2 * margin;
    int height = QApplication::desktop()->screenGeometry().height();
    setFixedWidth(width);
    setFixedHeight(MY_HEIGHT);
    show();
    move(margin, height - MY_HEIGHT - offset);
    return QDialog::exec();
}

void GlowLightControlDialog::createLayout()
{
    layout_.setContentsMargins(MY_WIDTH, 2, MY_WIDTH, 30);

    createSwitchView();
    createOKView();

    h_layout_.addWidget(&switch_view_);
    layout_.addLayout(&h_layout_);
    layout_.addSpacing(4);

//    title_.useTitleBarStyle();
    title_.setAlignment(Qt::AlignCenter);
    layout_.addWidget(&title_);

    // TODO may need different range
    slider_.setAutoFillBackground(true);
    slider_.setBackgroundRole(QPalette::Base);
    slider_.setRange(1, 130);
    slider_.setSingleStep(3);
    slider_.setOrientation(Qt::Horizontal);
    slider_.setFixedHeight(55);
    slider_.setStyleSheet(FANCY_STYLE);
    slider_.setValue(sys::SysStatus::instance().brightness());

    slider_h_layout_.setContentsMargins(30, 0, 30, 0);
    slider_h_layout_.addWidget(&slider_);

    layout_.addLayout(&slider_h_layout_);
    layout_.addSpacing(10);

    ok_h_layout_.setContentsMargins(450, 0, 0, 0);
    ok_h_layout_.addStretch(0);
    ok_h_layout_.addWidget(&ok_view_);
    layout_.addStretch(0);
    layout_.addLayout(&ok_h_layout_);

    connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

void GlowLightControlDialog::createSwitchView()
{
    switch_view_.setSubItemType(ui::CheckBoxView::type());
    switch_view_.setPreferItemSize(QSize(360, ITEM_HEIGHT));

    bool checked = sys::SysStatus::instance().glowLightOn();

    ODatas d;

    OData * item = new OData;
    item->insert(TAG_TITLE, tr("Turn on MOON Light"));
    item->insert(TAG_CHECKED, checked);
    d.push_back(item);

    switch_view_.setMinimumHeight( ITEM_HEIGHT );
    switch_view_.setMinimumWidth(360);
    switch_view_.setData(d, true);

    QObject::connect(&switch_view_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
                     this, SLOT(onSwitchClicked(CatalogView*, ContentView*, int)));

}

void GlowLightControlDialog::createOKView()
{
    ok_view_.setSubItemType(ui::CoverView::type());
    ok_view_.setPreferItemSize(QSize(-1, 40));
    ODatas d;

    OData * item = new OData;
    item->insert(TAG_TITLE, tr("OK"));
    d.push_back(item);

    ok_view_.setMinimumHeight( 42 );
    ok_view_.setFixedGrid(1, 1);
    ok_view_.setFixedWidth(160);
    ok_view_.setData(d, true);

    QObject::connect(&ok_view_, SIGNAL(itemActivated(CatalogView *, ContentView *, int)),
                     this, SLOT(onOkClicked()));
}

void GlowLightControlDialog::onSwitchClicked(CatalogView *catalog, ContentView *item, int user_data)
{
    if (!item || !item->data())
    {
        return;
    }

    bool checked = item->data()->value(TAG_CHECKED).toBool();
    item->data()->insert(TAG_CHECKED, !checked);

    sys::SysStatus::instance().turnGlowLightOn(!checked, true);

    update();
    onyx::screen::watcher().enqueue(catalog, onyx::screen::ScreenProxy::GU);
}

void GlowLightControlDialog::onOkClicked()
{
    qDebug() << "on ok clicked";
    accept();

    onyx::screen::watcher().removeWatcher(this);
    onyx::screen::watcher().enqueue(0, onyx::screen::ScreenProxy::GC);
}

void GlowLightControlDialog::updateText()
{
    title_.setText(tr("Brightness"));
}

void GlowLightControlDialog::onValueChanged(int v)
{
    qDebug() << "Value changed " << v;
    ODatas d;
    OData * item = new OData;
    item->insert(TAG_CHECKED, true);
    d.push_back(item);
    switch_view_.setData(d, true);
    sys::SysStatus::instance().setBrightness(v);

    onyx::screen::watcher().enqueue(&slider_, onyx::screen::ScreenProxy::DW);
}

void GlowLightControlDialog::keyPressEvent(QKeyEvent * ke)
{
    ke->accept();
    if (ke->key() == Qt::Key_Escape)
    {
        return;
    }
}

void GlowLightControlDialog::keyReleaseEvent(QKeyEvent * ke)
{
    ke->accept();
    if (ke->key() == Qt::Key_Escape)
    {
        reject();
        return;
    }
}

}  // namespace ui



