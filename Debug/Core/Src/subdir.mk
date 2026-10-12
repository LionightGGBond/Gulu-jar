################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AHT21_ENS160.c \
../Core/Src/BLE.c \
../Core/Src/LED.c \
../Core/Src/adc.c \
../Core/Src/conductivity\ sensor.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/kk_oled.c \
../Core/Src/kk_oled_bitmap.c \
../Core/Src/kk_oled_draw.c \
../Core/Src/kk_oled_driver.c \
../Core/Src/kk_oled_font.c \
../Core/Src/kk_ui.c \
../Core/Src/kk_ui_anim.c \
../Core/Src/kk_ui_app.c \
../Core/Src/kk_ui_dialog.c \
../Core/Src/kk_ui_draw.c \
../Core/Src/kk_ui_fonts.c \
../Core/Src/kk_ui_home.c \
../Core/Src/kk_ui_menu.c \
../Core/Src/kk_ui_nav.c \
../Core/Src/kk_ui_page.c \
../Core/Src/main.c \
../Core/Src/ph_sensor.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/ui_input.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/AHT21_ENS160.o \
./Core/Src/BLE.o \
./Core/Src/LED.o \
./Core/Src/adc.o \
./Core/Src/conductivity\ sensor.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/kk_oled.o \
./Core/Src/kk_oled_bitmap.o \
./Core/Src/kk_oled_draw.o \
./Core/Src/kk_oled_driver.o \
./Core/Src/kk_oled_font.o \
./Core/Src/kk_ui.o \
./Core/Src/kk_ui_anim.o \
./Core/Src/kk_ui_app.o \
./Core/Src/kk_ui_dialog.o \
./Core/Src/kk_ui_draw.o \
./Core/Src/kk_ui_fonts.o \
./Core/Src/kk_ui_home.o \
./Core/Src/kk_ui_menu.o \
./Core/Src/kk_ui_nav.o \
./Core/Src/kk_ui_page.o \
./Core/Src/main.o \
./Core/Src/ph_sensor.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/ui_input.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/AHT21_ENS160.d \
./Core/Src/BLE.d \
./Core/Src/LED.d \
./Core/Src/adc.d \
./Core/Src/conductivity\ sensor.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/kk_oled.d \
./Core/Src/kk_oled_bitmap.d \
./Core/Src/kk_oled_draw.d \
./Core/Src/kk_oled_driver.d \
./Core/Src/kk_oled_font.d \
./Core/Src/kk_ui.d \
./Core/Src/kk_ui_anim.d \
./Core/Src/kk_ui_app.d \
./Core/Src/kk_ui_dialog.d \
./Core/Src/kk_ui_draw.d \
./Core/Src/kk_ui_fonts.d \
./Core/Src/kk_ui_home.d \
./Core/Src/kk_ui_menu.d \
./Core/Src/kk_ui_nav.d \
./Core/Src/kk_ui_page.d \
./Core/Src/main.d \
./Core/Src/ph_sensor.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/ui_input.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/conductivity\ sensor.o: ../Core/Src/conductivity\ sensor.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/conductivity sensor.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AHT21_ENS160.cyclo ./Core/Src/AHT21_ENS160.d ./Core/Src/AHT21_ENS160.o ./Core/Src/AHT21_ENS160.su ./Core/Src/BLE.cyclo ./Core/Src/BLE.d ./Core/Src/BLE.o ./Core/Src/BLE.su ./Core/Src/LED.cyclo ./Core/Src/LED.d ./Core/Src/LED.o ./Core/Src/LED.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/conductivity\ sensor.cyclo ./Core/Src/conductivity\ sensor.d ./Core/Src/conductivity\ sensor.o ./Core/Src/conductivity\ sensor.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/kk_oled.cyclo ./Core/Src/kk_oled.d ./Core/Src/kk_oled.o ./Core/Src/kk_oled.su ./Core/Src/kk_oled_bitmap.cyclo ./Core/Src/kk_oled_bitmap.d ./Core/Src/kk_oled_bitmap.o ./Core/Src/kk_oled_bitmap.su ./Core/Src/kk_oled_draw.cyclo ./Core/Src/kk_oled_draw.d ./Core/Src/kk_oled_draw.o ./Core/Src/kk_oled_draw.su ./Core/Src/kk_oled_driver.cyclo ./Core/Src/kk_oled_driver.d ./Core/Src/kk_oled_driver.o ./Core/Src/kk_oled_driver.su ./Core/Src/kk_oled_font.cyclo ./Core/Src/kk_oled_font.d ./Core/Src/kk_oled_font.o ./Core/Src/kk_oled_font.su ./Core/Src/kk_ui.cyclo ./Core/Src/kk_ui.d ./Core/Src/kk_ui.o ./Core/Src/kk_ui.su ./Core/Src/kk_ui_anim.cyclo ./Core/Src/kk_ui_anim.d ./Core/Src/kk_ui_anim.o ./Core/Src/kk_ui_anim.su ./Core/Src/kk_ui_app.cyclo ./Core/Src/kk_ui_app.d ./Core/Src/kk_ui_app.o ./Core/Src/kk_ui_app.su ./Core/Src/kk_ui_dialog.cyclo ./Core/Src/kk_ui_dialog.d ./Core/Src/kk_ui_dialog.o ./Core/Src/kk_ui_dialog.su ./Core/Src/kk_ui_draw.cyclo ./Core/Src/kk_ui_draw.d ./Core/Src/kk_ui_draw.o ./Core/Src/kk_ui_draw.su ./Core/Src/kk_ui_fonts.cyclo ./Core/Src/kk_ui_fonts.d ./Core/Src/kk_ui_fonts.o ./Core/Src/kk_ui_fonts.su ./Core/Src/kk_ui_home.cyclo ./Core/Src/kk_ui_home.d ./Core/Src/kk_ui_home.o ./Core/Src/kk_ui_home.su ./Core/Src/kk_ui_menu.cyclo ./Core/Src/kk_ui_menu.d ./Core/Src/kk_ui_menu.o ./Core/Src/kk_ui_menu.su ./Core/Src/kk_ui_nav.cyclo ./Core/Src/kk_ui_nav.d ./Core/Src/kk_ui_nav.o ./Core/Src/kk_ui_nav.su ./Core/Src/kk_ui_page.cyclo ./Core/Src/kk_ui_page.d ./Core/Src/kk_ui_page.o ./Core/Src/kk_ui_page.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/ph_sensor.cyclo ./Core/Src/ph_sensor.d ./Core/Src/ph_sensor.o ./Core/Src/ph_sensor.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/ui_input.cyclo ./Core/Src/ui_input.d ./Core/Src/ui_input.o ./Core/Src/ui_input.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

