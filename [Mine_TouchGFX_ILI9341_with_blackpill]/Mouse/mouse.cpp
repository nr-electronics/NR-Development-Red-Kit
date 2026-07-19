/**
    ******************************************************************************
    * @file         mouse.cpp
    * @author       Ulmer Pavel
    * @email        ulmerpavel@mail.ru
    * @date         24 мая 2022 г.
    * @brief
    ******************************************************************************
    * @attention
    ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mouse.hpp"
#include "main.h"

CMouse& CMouse::instanse(void)
{
    static CMouse m;
    if(!m.inited)
    {
        m.init();
    }
    return m;
}

void CMouse::init(void)
{
    DisplayHeight = touchgfx::HAL::getInstance()->getDisplayHeight();
    DisplayWidth = touchgfx::HAL::getInstance()->getDisplayWidth();
    x_collector = 0;
    y_collector = 0;
    X_pos = DisplayWidth/2;
    Y_pos = DisplayHeight/2;
    inited = true;
}

inline void CMouse::preparePicture(touchgfx::Image& p)
{
    picture = &p;
    setPositionXY(X_pos, Y_pos);
    if(!picture->isVisible())
    {
        picture->setVisible(true);
    }
}

void CMouse::newMouseInfo(HID_MOUSE_Info_TypeDef* mouse_info)
{
    left_button_state = mouse_info->buttons[0];
    x_collector += static_cast<int8_t>(mouse_info->x);
    y_collector += static_cast<int8_t>(mouse_info->y);
}

void CMouse::process(touchgfx::Image& pic)
{
    if(!isPicturePrepare())
    {
        preparePicture(pic);
    }

    picture = &pic;

    if(isMouseMoved())
    {
        int16_t dx, dy;
        getMouseMovement(dx, dy);
        countXYpos(dx, dy);
        setPositionXY(X_pos, Y_pos);
    }
}

inline bool CMouse::isPicturePrepare(void)
{
    return (static_cast<bool>(picture));
}

inline bool CMouse::isMouseMoved(void)
{
    return (x_collector!=0 || y_collector!=0);
}

inline void CMouse::countXYpos(int16_t dx, uint16_t dy)
{
    X_pos += dx*SENSIVITY;
    Y_pos += dy*SENSIVITY;
    if(X_pos < 0) X_pos = 0;
    if(X_pos > DisplayWidth-2) X_pos = DisplayWidth-2;
    if(Y_pos < 0) Y_pos = 0;
    if(Y_pos > DisplayHeight-2)Y_pos = DisplayHeight-2;
}

inline void CMouse::getMouseMovement(int16_t& dx, int16_t& dy)
{
    __disable_irq();
    dx = x_collector;
    dy = y_collector;
    x_collector = 0;
    y_collector = 0;
    __enable_irq();
}

inline void CMouse::setPositionXY(uint16_t x, uint16_t y)
{
    if(!picture)
    {
        return;
    }

    picture->moveTo(x, y);
}

bool CMouse::leftButtonState(int32_t& x, int32_t& y)
{
    x = X_pos;
    y = Y_pos;
    return left_button_state;
}
/*******************************END OF FILE************************************/
