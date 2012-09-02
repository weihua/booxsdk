
#ifndef CATALAN_KEYBOARD_DATA_H_
#define CATALAN_KEYBOARD_DATA_H_

#include "keyboard_data.h"

namespace ui
{

class CatalanKeyboardData: public KeyboardData
{
public:
    CatalanKeyboardData();
    virtual ~CatalanKeyboardData();

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


#endif /* CATALAN_KEYBOARD_DATA_H_ */
