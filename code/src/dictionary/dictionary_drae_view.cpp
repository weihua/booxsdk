/*
 * dictionary_rae_view.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: weihuahuang
 */

#include "onyx/dictionary/dictionary_drae_view.h"
#include "onyx/screen/screen_update_watcher.h"

DictionaryDraeView::DictionaryDraeView(QWidget *parent)
    : QWebView(parent)
{
    page_ = new QWebPage(this);
    setPage(page_);

    this->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    this->page()->mainFrame()->setScrollBarValue(Qt::Vertical,100);

    load(QUrl("/usr/share/dictionary_RAE/dict_drae_legal_agreement.html"));
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::GU);
    resize(600, 800);
}

DictionaryDraeView::~DictionaryDraeView()
{

}

void DictionaryDraeView::mousePressEvent(QMouseEvent *event)
{
    position_ = event->pos();

    return QWebView::mousePressEvent(event);
}

void DictionaryDraeView::mouseReleaseEvent(QMouseEvent *me)
{
    QPoint delta = position_ - me->pos();
    me->accept();
    accurateScroll(delta.x(), delta.y());

    QWebView::mouseReleaseEvent(me);
    onyx::screen::watcher().enqueue(this, onyx::screen::ScreenProxy::A2);
}

void DictionaryDraeView::accurateScroll(int dx, int dy)
{
    int key_code = Qt::Key_PageDown;
    if (dy < 0)
    {
        key_code = Qt::Key_PageUp;
    }

    page()->currentFrame()->scroll(dx, dy);
    updateViewportRange();
}

void DictionaryDraeView::updateViewportRange()
{
    QSizeF s = page()->currentFrame()->contentsSize();
    page()->currentFrame()->scrollPosition();
}

void DictionaryDraeView::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}
