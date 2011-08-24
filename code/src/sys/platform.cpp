
#include "onyx/sys/platform.h"
#include <QtCore/qglobal.h>

namespace sys
{
static const QString IM31L = "imx31L";
static const QString MARVELL = "166e";
static const QString IMX508 = "imx508";

QString platform()
{
    static QString p;
    if (p.isEmpty())
    {
        p = qgetenv("PLATFORM");
        if (p.isEmpty())
        {
            p = IM31L;
        }
    }
    return p;
}

bool isIMX31L()
{
    return platform() == IM31L;
}

bool is166E()
{
    return platform() == MARVELL;
}

bool isImx508()
{
    return platform() == IMX508;
}

QString soundModule()
{
    static QString mod = qgetenv("SOUND_MODULE");
    if (mod.isEmpty())
    {
        if (isIMX31L())
        {
            mod = "snd-soc-imx-3stack-wm8711";
        }
        else if (is166E())
        {
            mod = "snd_soc_booxe";
        }
        else if (isImx508())
        {
            mod = "snd_soc_imx_3stack_wm8976";
        }
        else
        {
            qDebug("No sound module found, check profile.");
        }
    }
    return mod;
}

int defaultRotation()
{
    static int rotation = -1;
    if (rotation < 0)
    {
        rotation = qgetenv("DEFAULT_ROTATION").toInt();
    }
    return rotation;
}

int batteryPercentageThreshold()
{
    return 5;
}

}    // namespace sys

