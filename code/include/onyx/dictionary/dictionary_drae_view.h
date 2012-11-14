/*
 * dictionary_rae_view.h
 *
 *  Created on: Nov 14, 2012
 *      Author: weihuahuang
 */

#ifndef DICTIONARY_RAE_VIEW_H_
#define DICTIONARY_RAE_VIEW_H_

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QtGui/QtGui>
#include "onyx/ui/ui.h"

class DictionaryDraeView : public QWebView
{
    Q_OBJECT

public:
    DictionaryDraeView(QWidget *parent=0);
    ~DictionaryDraeView();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void accurateScroll(int dx, int dy);
    void updateViewportRange();

    QPoint  position_;
    QWebPage *page_;

};

#endif /* DICTIONARY_RAE_VIEW_H_ */
