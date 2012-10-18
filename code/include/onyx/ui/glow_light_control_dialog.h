#ifndef GLOW_LIGHT_CONTROL_DIALOG_H_
#define GLOW_LIGHT_CONTROL_DIALOG_H_

#include "onyx/ui/onyx_dialog.h"
#include "onyx/sys/sys.h"
#include "onyx/ui/catalog_view.h"

namespace ui
{

/// Clock dialog.
class GlowLightControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlowLightControlDialog(QWidget *parent = 0);
    ~GlowLightControlDialog(void);

    int exec();

public Q_SLOTS:
    void onValueChanged(int v);

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

private Q_SLOTS:
    void onSwitchClicked(CatalogView *catalog, ContentView *item, int user_data);
    void onOkClicked();

private:
    void createLayout();
    void updateText();
    void createSwitchView();
    void createOKView();

private:
    QVBoxLayout layout_;
    QHBoxLayout slider_h_layout_;
    QHBoxLayout h_layout_;
    QHBoxLayout ok_h_layout_;
    OnyxLabel title_;
    QSlider slider_;

    CatalogView switch_view_;
    CatalogView ok_view_;

};


}  // namespace ui


#endif  // GLOW_LIGHT_CONTROL_DIALOG_H_
