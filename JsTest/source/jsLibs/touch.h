#pragma once
#include <stdio.h>
#include <iostream>
#include <3ds.h>

class TouchScreen
{
    private:
        touchPosition currentPos;
    public: 
        void Refresh();
        u16 get_x();
        u16 get_y();
};

#include "FastLibDefine.h"
EXTERN_LIBDEFINE(TouchScreen);