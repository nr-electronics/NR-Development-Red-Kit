/**
    ******************************************************************************
    * @file         mouse.hpp
    * @author       Ulmer Pavel
    * @email        ulmerpavel@mail.ru
    * @date         24 мая 2022 г.
    * @brief
    ******************************************************************************
    * @attention
    ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INC_MOUSE_MOUSE_HPP_
#define __INC_MOUSE_MOUSE_HPP_


/* Includes ------------------------------------------------------------------*/
#include <../../Middlewares/ST/touchgfx/framework/include/touchgfx/widgets/Image.hpp>
#include <../../Middlewares/ST/touchgfx/framework/include/touchgfx/hal/HAL.hpp>
#include <usbh_hid_mouse.h>
/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


class CMouse
{
    public:
        static CMouse& instanse(void);

        /**
         * @fn process(touchgfx::Image& p);
         *
         * @brief Mouse main process method.
         *        Call it from screen handleTickEvent.
         *
         * @param [in] p Alias to cursor picture.
         */
        void process(touchgfx::Image& p);

        /**
         * @fn newMouseInfo(HID_MOUSE_Info_TypeDef* mouse_info);
         *
         * @brief Mouse event.
         *
         * @param [in] mouse_info Pointer mouse event structure.
         */
        void newMouseInfo(HID_MOUSE_Info_TypeDef* mouse_info);

        /**
         * @fn leftButtonState(int32_t& x, int32_t& y);
         *
         * @brief Get coordinates of cursor and state of left button.
         *        Call it from STM32TouchController.hpp (method sampleTouch)
         *
         * @param [out] x,y Coordinates of cursor.
         * @retval State of left button.
         */
        bool leftButtonState(int32_t& x, int32_t& y);

    private:
        CMouse() = default;
        bool inited = false;
        touchgfx::Image* picture = nullptr;
        uint16_t DisplayHeight = 0;
        uint16_t DisplayWidth = 0;
        int16_t X_pos, Y_pos;
        static constexpr int16_t SENSIVITY = 1;
        int16_t x_collector = 0;
        int16_t y_collector = 0;
        bool left_button_state = false;
        void init(void);
        void preparePicture(touchgfx::Image& p);
        void setPositionXY(uint16_t x, uint16_t y);
        void getMouseMovement(int16_t& dx, int16_t& dy);
        void countXYpos(int16_t dx, uint16_t dy);
        bool isMouseMoved(void);
        bool isPicturePrepare(void);
};

#endif /* __INC_MOUSE_MOUSE_HPP_ */

/*******************************END OF FILE************************************/
