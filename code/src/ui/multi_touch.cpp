
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
    dirty_ = true;

    // Just touched.
    if (prev == 0)
    {
        pixmap_.reset(new QPixmap(QPixmap::grabWidget(wnd, wnd->rect())));
        rc_touched_.setCoords(r1.center().x(), r1.center().y(), r2.center().x(), r2.center().y());
        zoom_ = 1.0;
    }
    else
    {
        QRect ra;
        ra.setCoords(r1.center().x(), r1.center().y(), r2.center().x(), r2.center().y());
        zoom_ = sqrt(static_cast<qreal>(diagonal(ra)) / static_cast<qreal>(diagonal(rc_touched_)));
        *pixmap_ = pixmap_->scaled(pixmap_->width() * zoom_, pixmap_->height() * zoom_);
        rc_touched_ = ra;
    }
    sys::SysStatus::instance().requestMultiTouch();
}

int MultiTouch::diagonal(const QRect & rc)
{
    int l = rc.width() * rc.width() + rc.height() * rc.height();
    return static_cast<int>(sqrt(static_cast<qreal>(l)));
}

void MultiTouch::onMultiTouchReleaseDetected(QRect r1, QRect r2)
{
    pixmap_.reset(0);
}

QPixmap * MultiTouch::pixmap()
{
    return pixmap_.get();
}


