#ifndef GEORGIAN_KEYBOARD_LAYOUT_H
#define GEORGIAN_KEYBOARD_LAYOUT_H

#include "keyboard_layout.h"

namespace ui
{

class GeorgianKeyboardLayout: public KeyboardLayout
{
public:
    GeorgianKeyboardLayout();
    virtual ~GeorgianKeyboardLayout();

    virtual QSize getKeySize(int code);

protected:
    virtual void initCode();
    virtual void initKeys();
};

}

#endif // GEORGIAN_KEYBOARD_LAYOUT_H
