#include "touch.h"
#include <dukglue/dukglue.h>

LIBDEFINE(TouchScreen);

void TouchScreen::Refresh()
{
    hidTouchRead(&currentPos);
}

u16 TouchScreen::get_x()
{
    return currentPos.px;
}

u16 TouchScreen::get_y()
{
    return currentPos.py;
}
