/**
    ******************************************************************************
    * @file         mouse_cport.cpp
    * @author       Ulmer Pavel
    * @email        ulmerpavel@mail.ru
    * @date         24 мая 2022 г.
    * @brief
    ******************************************************************************
    * @attention
    ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mouse_cport.h"
#include "mouse.hpp"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
    HID_MOUSE_Info_TypeDef *MOUSE_Info = USBH_HID_GetMouseInfo(phost);
    CMouse::instanse().newMouseInfo(MOUSE_Info);
}




/*******************************END OF FILE************************************/
