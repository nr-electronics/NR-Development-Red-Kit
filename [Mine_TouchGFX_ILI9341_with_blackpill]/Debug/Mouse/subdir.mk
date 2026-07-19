################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Mouse/mouse.cpp \
../Mouse/mouse_cport.cpp 

OBJS += \
./Mouse/mouse.o \
./Mouse/mouse_cport.o 

CPP_DEPS += \
./Mouse/mouse.d \
./Mouse/mouse_cport.d 


# Each subdirectory must supply rules for building sources it contributes
Mouse/%.o Mouse/%.su Mouse/%.cyclo: ../Mouse/%.cpp Mouse/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../TouchGFX/App -I../Mouse -I../TouchGFX/target/generated -I../TouchGFX/target -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mouse

clean-Mouse:
	-$(RM) ./Mouse/mouse.cyclo ./Mouse/mouse.d ./Mouse/mouse.o ./Mouse/mouse.su ./Mouse/mouse_cport.cyclo ./Mouse/mouse_cport.d ./Mouse/mouse_cport.o ./Mouse/mouse_cport.su

.PHONY: clean-Mouse

