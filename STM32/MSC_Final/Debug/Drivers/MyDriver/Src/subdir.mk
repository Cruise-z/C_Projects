################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MyDriver/Src/W25QXX.c 

C_DEPS += \
./Drivers/MyDriver/Src/W25QXX.d 

OBJS += \
./Drivers/MyDriver/Src/W25QXX.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MyDriver/Src/%.o Drivers/MyDriver/Src/%.su Drivers/MyDriver/Src/%.cyclo: ../Drivers/MyDriver/Src/%.c Drivers/MyDriver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Data/Work/STM32/MSC_Final/Drivers/MyDriver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MyDriver-2f-Src

clean-Drivers-2f-MyDriver-2f-Src:
	-$(RM) ./Drivers/MyDriver/Src/W25QXX.cyclo ./Drivers/MyDriver/Src/W25QXX.d ./Drivers/MyDriver/Src/W25QXX.o ./Drivers/MyDriver/Src/W25QXX.su

.PHONY: clean-Drivers-2f-MyDriver-2f-Src

