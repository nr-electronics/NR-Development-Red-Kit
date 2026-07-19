/**
    ******************************************************************************
    * @file         mouse_cport.h
    * @author       Ulmer Pavel
    * @email        ulmerpavel@mail.ru
    * @date         24 мая 2022 г.
    * @brief
    ******************************************************************************
    * @attention
    ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INC_MOUSE_MOUSE_CPORT_H_
#define __INC_MOUSE_MOUSE_CPORT_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <usbh_hid_mouse.h>
/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

 void USBH_HID_EventCallback(USBH_HandleTypeDef *phost);

#ifdef __cplusplus
}
#endif

#endif /* __INC_MOUSE_MOUSE_CPORT_H_ */

/*******************************END OF FILE************************************/
