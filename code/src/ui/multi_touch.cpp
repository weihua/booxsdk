
#include "onyx/ui/multi_touch.h"
#include "onyx/sys/sys_status.h"

MultiTouch::MultiTouch()
{
}

MultiTouch::~MultiTouch()
{
}

void MultiTouch::onMultiTouchHoldDetected(QWidget *wnd, QRect r1, QRect r2, int prev, int now)
{
    rc_touched_.setCoords(r1.center().x(), r1.center().y(), r2.center().x(), r2.center().y());
    rc_touched_ = rc_touched_.normalized();

    if (prev == 0)
    {
        if (!band_)
        {
            band_.reset(new QRubberBand(QRubberBand::Rectangle, wnd));
            band_->setGeometry(rc_touched_);
            band_->show();
        }
    }
    else
    {
        band_->setGeometry(rc_touched_);
    }
    sys::SysStatus::instance().requestMultiTouch();
}


void MultiTouch::onMultiTouchReleaseDetected(QRect r1, QRect r2)
{
    if (band_)
    {
        band_->hide();
        band_.reset(0);
    }
}

