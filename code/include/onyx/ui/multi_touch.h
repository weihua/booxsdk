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

    QRubberBand * band() { return band_.get(); }

private:
    scoped_ptr<QRubberBand> band_;
    QRect rc_touched_;
};

#endif

