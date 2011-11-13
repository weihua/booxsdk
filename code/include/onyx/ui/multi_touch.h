#ifndef MULTI_TOUCH_H_
#define MULTI_TOUCH_H_

#include <QtGui/QtGui>
#include "onyx/base/base.h"


/// Class for multi touch. Caller can use this class to
/// calculate the new zooming factor and new image position.
/// This class generates a low quality image during multi touch holding.
class MultiTouch
{
public:
    MultiTouch();
    ~MultiTouch();

public:
    void onMultiTouchHoldDetected(QWidget *wnd, QRect r1, QRect r2, int prev, int now);
    void onMultiTouchReleaseDetected(QRect r1, QRect r2);

    QPixmap * pixmap();
    QPoint position();

    int diagonal(const QRect & rc);

private:
    scoped_ptr<QPixmap> pixmap_;
    QPixmap result_;
    QRect rc_touched_;
    qreal zoom_;
    bool dirty_;
};

#endif

