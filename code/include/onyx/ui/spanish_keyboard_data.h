#ifndef ONYX_SPANISH_KEYBOARD_LAYOUT_H_
#define ONYX_SPANISH_KEYBOARD_LAYOUT_H_

#include "keyboard_data.h"

namespace ui
{

class SpanishKeyboardData: public KeyboardData
{
public:
    SpanishKeyboardData();
    virtual ~SpanishKeyboardData();

protected:
    virtual void initTopKeyCode();
    virtual void initLeftKeyCode();
    virtual void initMiddleKeyCode();
    virtual void initRightKeyCode();
    virtual void initBottomKeyCode();

    virtual void initTopKeyShiftCode();
    virtual void initLeftKeyShiftCode();
    virtual void initMiddleKeyShiftCode();
    virtual void initRightKeyShiftCode();
    virtual void initBottomKeyShiftCode();

    virtual void initLeftKeySymbolCode();
    virtual void initMiddleKeySymbolCode();
    virtual void initRightKeySymbolCode();
};

};

#endif
