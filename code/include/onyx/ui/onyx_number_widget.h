#ifndef ONYX_NUMBER_WIDGET_H
#define ONYX_NUMBER_WIDGET_H

#include "context_dialog_base.h"
#include "onyx/ui/catalog_view.h"
#include "onyx/ui/content_view.h"
#include "onyx/ui/factory.h"
#include "onyx/data/data_tags.h"

namespace ui
{

class ButtonFactory : public Factory
{
public:
    ButtonFactory();
    virtual ~ButtonFactory();

public:
    virtual ContentView * createView(QWidget *parent, const QString &type = QString());
};

class ButtonView : public ContentView
{
        Q_OBJECT

    public:
        ButtonView(QWidget *parent);
        virtual ~ButtonView();

        static const QString type();

    public:
        virtual void updateView();

    protected:
        void paintEvent(QPaintEvent * event);

        void keyPressEvent(QKeyEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    private:
        QLabel label_title_;

        void drawTitle(QPainter & painter, QRect rect);
};

class OnyxNumberWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OnyxNumberWidget(QWidget *parent);
    ~OnyxNumberWidget(void);
    void setOkButtonFocus(void);

Q_SIGNALS:
    void okClicked();
    void keyPress(QKeyEvent *);

protected:
    void keyReleaseEvent(QKeyEvent *);

private:
    void createLayout();

private Q_SLOTS:
    void onItemActivated(CatalogView *, ContentView *, int);

private:
    CatalogView         buttons_;        ///< All number buttons.
    QVBoxLayout number_layout_;  ///< Layout.

private:
    NO_COPY_AND_ASSIGN(OnyxNumberWidget);
};

};      // namespace ui

#endif // ONYX_NUMBER_WIDGET_H
